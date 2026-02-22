#include <iostream>
#include <vector>
#include <fstream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <cerrno>
#include <chrono>
#include <thread>

#define MAX_SESSIONS 100
#define MAX_DATA (50 * 1024 * 1024)
#define TIMEOUT_SECONDS 20

struct SharedBlock
{
    size_t key_size;
    size_t data_size;
    int status;  // 0 - available, 1 - busy, 2 - response is ready, -1 - error
    uint8_t buffer[MAX_DATA];
};

std::vector<uint8_t> read_file(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open input file: " + path);
    }

    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), 
        std::istreambuf_iterator<char>());
}

void write_file(const std::string& path, const std::vector<uint8_t>& data)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open output file: " + path);
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    if (!file)
    {
        throw std::runtime_error("Failed to write output file");
    }
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "Usage:\n";
        std::cout << "./client encrypt input output key\n";
        std::cout << "./client decrypt input output key\n";
        return 1;
    }

    std::string input = argv[1];
    std::string output = argv[2];
    std::string key_str = argv[3];

    try
    {
        std::cout << "[CLIENT] Reading file: " << input << "\n";

        std::vector<uint8_t> data = read_file(input);

        std::cout << "[CLIENT] File size: " << data.size() << " bytes\n";

        if (data.size() > MAX_DATA)
        {
            throw std::runtime_error("File exceeds server limit of " + std::to_string(MAX_DATA) + " bytes");
        }

        std::vector<uint8_t> key(key_str.begin(), key_str.end());
        
        if (key.size() + data.size() > MAX_DATA)
        {
            throw std::runtime_error("Combined key and data size exceeds server limit");
        }

        std::cout << "[CLIENT] Key size: " << key.size() << " bytes\n";
        std::cout << "[CLIENT] Total data to send: " << (key.size() + data.size()) << " bytes\n";

        for (int id = 0; id < MAX_SESSIONS; id++)
        {
            std::string shm_name = "/rc4_session_" + std::to_string(id);
            int shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);

            if (shm_fd == -1)
            {
                continue;
            }

            void* ptr = mmap(nullptr, sizeof(SharedBlock), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

            if (ptr == MAP_FAILED)
            {
                std::cerr << "[CLIENT] mmap failed for session " << id << ": " << strerror(errno) << "\n";
                close(shm_fd);
                continue;
            }

            SharedBlock* block = static_cast<SharedBlock*>(ptr);

            if (__sync_bool_compare_and_swap(&block->status, 0, 1))
            {
                std::cout << "[CLIENT] Using session " << id << "\n";

                block->key_size = key.size();
                block->data_size = data.size();
                
                memcpy(block->buffer, key.data(), key.size());
                memcpy(block->buffer + key.size(), data.data(), data.size());
                
                std::string sem_req_name = "/rc4_req_" + std::to_string(id);
                std::string sem_resp_name = "/rc4_resp_" + std::to_string(id);

                sem_t* sem_req = sem_open(sem_req_name.c_str(), 0);
                sem_t* sem_resp = sem_open(sem_resp_name.c_str(), 0);

                if (sem_req == SEM_FAILED || sem_resp == SEM_FAILED)
                {
                    std::cerr << "[CLIENT] sem_open failed for session " << id << ": " << strerror(errno) << "\n";
                    block->status = 0;
                    munmap(ptr, sizeof(SharedBlock));
                    close(shm_fd);
                    continue;
                }

                sem_post(sem_req);
                std::cout << "[CLIENT] Request sent, waiting for response...\n";

                auto start = std::chrono::steady_clock::now();
                bool received = false;

                while (true)
                {
                    if (sem_trywait(sem_resp) == 0)
                    {
                        received = true;
                        break;
                    }

                    if (errno != EAGAIN)
                    {
                        std::cerr << "[CLIENT] sem_trywait error: " << strerror(errno) << "\n";
                        break;
                    }

                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

                    if (elapsed >= TIMEOUT_SECONDS)
                    {
                        break;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

                if (!received)
                {
                    std::cerr << "[CLIENT] Timeout waiting for server on session " << id << "\n";
                    block->status = 0;
                    sem_close(sem_req);
                    sem_close(sem_resp);
                    munmap(ptr, sizeof(SharedBlock));
                    close(shm_fd);
                    return 1;
                }

                if (block->status == -1)
                {
                    throw std::runtime_error("Server processing error on session " + std::to_string(id));
                }

                std::vector<uint8_t> result(block->buffer, block->buffer + block->data_size);

                write_file(output, result);

                std::cout << "[CLIENT] Output written to: " << output << "\n";
                std::cout << "[CLIENT] Operation completed successfully\n";

                block->status = 0;

                sem_close(sem_req);
                sem_close(sem_resp);
                munmap(ptr, sizeof(SharedBlock));
                close(shm_fd);
                
                return 0;
            }

            munmap(ptr, sizeof(SharedBlock));
            close(shm_fd);
        }

        throw std::runtime_error("No available sessions found");
    }
    catch (const std::exception& e)
    {
        std::cerr << "[CLIENT ERROR] " << e.what() << "\n";
        return 1;
    }
}