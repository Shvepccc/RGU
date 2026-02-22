#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <csignal>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <cerrno>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include "../include/rc4.hpp"

#define MAX_SESSIONS 100
#define INITIAL_SESSIONS 16
#define MAX_DATA (50 * 1024 * 1024)

struct SharedBlock
{
    size_t key_size;
    size_t data_size;
    int status;     // 0 - available, 1 - busy, 2 - response is ready, -1 - error
    uint8_t buffer[MAX_DATA];
};

struct SessionContext
{
    int id;
    int shm_fd;
    SharedBlock* block;
    sem_t* sem_req;
    sem_t* sem_resp;
    std::thread worker;
    bool running;
};

std::atomic<bool> running(true);
std::mutex pool_mutex;
std::vector<SessionContext> sessions;
int current_sessions = 0;
int growth_step = 16;

void signal_handler(int)
{
    std::cout << "\n[SERVER] Shutting down...\n";
    running = false;
}

void session_worker(int id, SharedBlock* block, sem_t* sem_req, sem_t* sem_resp)
{
    std::cout << "[SERVER] Worker for session " << id << " started\n";
    
    while (running)
    {
        if (sem_trywait(sem_req) == 0)
        {
            std::cout << "[SERVER] Session " << id << " processing request\n";
            
            if (block->key_size + block->data_size > MAX_DATA)
            {
                std::cerr << "[SERVER] Session " << id << ": data too large\n";
                block->status = -1;
                sem_post(sem_resp);
                continue;
            }
            
            try
            {
                std::vector<uint8_t> key(block->buffer, block->buffer + block->key_size);
                std::vector<uint8_t> data(block->buffer + block->key_size, 
                    block->buffer + block->key_size + block->data_size);
                
                RC4 rc4(key);
                rc4.process(data);
                
                memcpy(block->buffer, data.data(), data.size());
                
                block->status = 2;
                std::cout << "[SERVER] Session " << id << " completed, size=" << data.size() << " bytes\n";
            }
            catch (const std::exception& e)
            {
                std::cerr << "[SERVER] Session " << id << " error: " << e.what() << "\n";
                block->status = -1;
            }
            sem_post(sem_resp);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    
    std::cout << "[SERVER] Worker for session " << id << " stopped\n";
}

bool create_session(int id)
{
    std::string shm_name = "/rc4_session_" + std::to_string(id);
    std::string sem_req_name = "/rc4_req_" + std::to_string(id);
    std::string sem_resp_name = "/rc4_resp_" + std::to_string(id);

    shm_unlink(shm_name.c_str());
    sem_unlink(sem_req_name.c_str());
    sem_unlink(sem_resp_name.c_str());

    int shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cerr << "[SERVER] shm_open failed for session " << id << ": " << strerror(errno) << "\n";
        return false;
    }

    if (ftruncate(shm_fd, sizeof(SharedBlock)) == -1)
    {
        std::cerr << "[SERVER] ftruncate failed for session " << id << ": " << strerror(errno) << "\n";
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }

    void* ptr = mmap(nullptr, sizeof(SharedBlock), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        std::cerr << "[SERVER] mmap failed for session " << id << ": " << strerror(errno) << "\n";
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }

    SharedBlock* block = static_cast<SharedBlock*>(ptr);
    memset(block, 0, sizeof(SharedBlock));
    block->status = 0;

    sem_t* sem_req = sem_open(sem_req_name.c_str(), O_CREAT | O_EXCL, 0666, 0);
    if (sem_req == SEM_FAILED)
    {
        std::cerr << "[SERVER] sem_open(req) failed for session " << id << ": " << strerror(errno) << "\n";
        munmap(ptr, sizeof(SharedBlock));
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }

    sem_t* sem_resp = sem_open(sem_resp_name.c_str(), O_CREAT | O_EXCL, 0666, 0);
    if (sem_resp == SEM_FAILED)
    {
        std::cerr << "[SERVER] sem_open(resp) failed for session " << id << ": " << strerror(errno) << "\n";
        sem_close(sem_req);
        sem_unlink(sem_req_name.c_str());
        munmap(ptr, sizeof(SharedBlock));
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }

    SessionContext ctx;
    ctx.id = id;
    ctx.shm_fd = shm_fd;
    ctx.block = block;
    ctx.sem_req = sem_req;
    ctx.sem_resp = sem_resp;
    ctx.running = true;
    
    ctx.worker = std::thread(session_worker, id, block, sem_req, sem_resp);
    
    {
        std::lock_guard<std::mutex> lock(pool_mutex);
        sessions.push_back(std::move(ctx));
    }
    
    std::cout << "[SERVER] Session " << id << " created successfully\n";
    return true;
}

void destroy_session(int id)
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    
    for (auto it = sessions.begin(); it != sessions.end(); ++it)
    {
        if (it->id == id)
        {
            it->running = false;
            
            if (it->worker.joinable())
            {
                it->worker.join();
            }
            
            munmap(it->block, sizeof(SharedBlock));
            close(it->shm_fd);
            sem_close(it->sem_req);
            sem_close(it->sem_resp);
            
            std::string shm_name = "/rc4_session_" + std::to_string(id);
            std::string sem_req_name = "/rc4_req_" + std::to_string(id);
            std::string sem_resp_name = "/rc4_resp_" + std::to_string(id);
            
            shm_unlink(shm_name.c_str());
            sem_unlink(sem_req_name.c_str());
            sem_unlink(sem_resp_name.c_str());
            
            sessions.erase(it);
            std::cout << "[SERVER] Session " << id << " destroyed\n";
            break;
        }
    }
}

int count_busy_sessions()
{
    int busy = 0;
    
    for (const auto& session : sessions)
    {
        if (session.block->status == 1)
        {
            busy++;
        }
    }
    
    return busy;
}

void scale_sessions_if_needed()
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    
    if (current_sessions == 0)
    {
        return;
    }
    
    int busy = count_busy_sessions();
    double load = (double)busy / current_sessions;
    
    if (load >= 0.8 && current_sessions < MAX_SESSIONS)
    {
        int to_create = std::min(growth_step, MAX_SESSIONS - current_sessions);
        
        std::cout << "[SERVER] Scaling: +" << to_create << " sessions (load=" << load << ")\n";
        
        for (int i = 0; i < to_create; i++)
        {
            if (create_session(current_sessions + i))
            {
                current_sessions++;
            }
        }
        
        growth_step *= 2;
    }
}

int main()
{
    std::cout << "=== RC4 Encryption Server (Dynamic Pool) ===\n";
    std::cout << "Initial sessions: " << INITIAL_SESSIONS << "\n";

    signal(SIGINT, signal_handler);

    for (int i = 0; i < INITIAL_SESSIONS; i++)
    {
        if (create_session(i))
        {
            current_sessions++;
        }
        else
        {
            std::cerr << "[SERVER] Failed to create initial session " << i << "\n";
        }
    }

    std::cout << "[SERVER] Created " << current_sessions << " initial sessions\n";
    std::cout << "[SERVER] Waiting for connections...\n";

    while (running)
    {
        scale_sessions_if_needed();
        
        {
            std::lock_guard<std::mutex> lock(pool_mutex);
            for (auto& session : sessions)
            {
                if (session.block->status == 2 || session.block->status == -1)
                {
                    session.block->status = 0;
                }
            }
        }
        sleep(1);
    }

    std::cout << "[SERVER] Stopping all sessions...\n";
    
    std::vector<int> ids;
    {
        std::lock_guard<std::mutex> lock(pool_mutex);
        for (const auto& session : sessions)
        {
            ids.push_back(session.id);
        }
    }
    
    for (int id : ids)
    {
        destroy_session(id);
    }
    
    std::cout << "[SERVER] Shutdown complete\n";
    return 0;
}