#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <random>
#include <signal.h>
#include <boost/multiprecision/cpp_int.hpp>

#include "../include/modular_arithmetic.hpp"
#include "../include/rijndael/rijndael_algorithm.hpp"
#include "../include/mars/mars_algorithm.hpp"
#include "../include/des_algorithm.hpp"
#include "../include/diffie_hellman.hpp"

static volatile sig_atomic_t server_socket_fd = -1;
static volatile sig_atomic_t client_socket_fd = -1;

void cleanup_sockets()
{
    if (client_socket_fd != -1)
    {
        close(client_socket_fd);
        client_socket_fd = -1;
    }
    if (server_socket_fd != -1)
    {
        close(server_socket_fd);
        server_socket_fd = -1;
    }
}

void signal_handler(int sig)
{
    std::cout << "\n[ERROR] signal " << sig << " received, cleaning up..." << std::endl;
    cleanup_sockets();
    exit(128 + sig);
}

std::vector<std::uint8_t> generate_random_bytes(std::size_t b_size)
{
    std::vector<std::uint8_t> result(b_size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::uint8_t> dist(0, 255);
    
    for (std::size_t i = 0; i < b_size; ++i) {
        result[i] = dist(gen);
    }
    
    return result;
}

std::vector<std::uint8_t> extract_key_bytes(boost::multiprecision::cpp_int shared_secret, std::size_t key_size)
{
    std::vector<std::uint8_t> key_bytes;
    for (std::size_t i = 0; i < key_size; ++i)
    {
        key_bytes.push_back(static_cast<std::uint8_t>((shared_secret >> (i * 8)) & 0xFF));
    }
    return key_bytes;
}

void send_bigint(int socket_fd, const boost::multiprecision::cpp_int& value)
{
    std::string s = value.str();
    std::uint32_t len = static_cast<std::uint32_t>(s.length());
    if (send(socket_fd, &len, sizeof(len), 0) < 0) {
        std::cout << "[ERROR] failed to send bigint length" << std::endl;
        return;
    }
    if (send(socket_fd, s.c_str(), len, 0) < 0) {
        std::cout << "[ERROR] failed to send bigint data" << std::endl;
        return;
    }
    std::cout << "[DEBUG] bigint sent" << std::endl;
}

boost::multiprecision::cpp_int receive_bigint(int socket_fd)
{
    std::uint32_t len = 0;
    if (recv(socket_fd, &len, sizeof(len), 0) <= 0) {
        std::cout << "[ERROR] failed to receive bigint length" << std::endl;
        return -1;
    }
    std::vector<char> buffer(len + 1, 0);
    if (recv(socket_fd, buffer.data(), len, 0) <= 0) {
        std::cout << "[ERROR] failed to receive bigint data" << std::endl;
        return -1;
    }
    std::cout << "[DEBUG] bigint received" << std::endl;
    return boost::multiprecision::cpp_int(buffer.data());
}

void send_buffer(int socket_fd, const std::vector<std::uint8_t>& buffer)
{
    std::uint32_t len = static_cast<std::uint32_t>(buffer.size());
    if (send(socket_fd, &len, sizeof(len), 0) < 0) {
        std::cout << "[ERROR] failed to send buffer length" << std::endl;
        return;
    }
    if (send(socket_fd, buffer.data(), len, 0) < 0) {
        std::cout << "[ERROR] failed to send buffer data" << std::endl;
        return;
    }
    std::cout << "[DEBUG] buffer sent" << std::endl;
}

std::vector<std::uint8_t> receive_buffer(int socket_fd)
{
    std::uint32_t len = 0;
    if (recv(socket_fd, &len, sizeof(len), 0) <= 0) {
        std::cout << "[ERROR] failed to receive buffer length" << std::endl;
        return {};
    }
    std::vector<std::uint8_t> buffer(len);
    if (recv(socket_fd, buffer.data(), len, 0) <= 0) {
        std::cout << "[ERROR] failed to receive buffer data" << std::endl;
        return {};
    }
    std::cout << "[DEBUG] buffer received" << std::endl;
    return buffer;
}

void perform_dh_and_crypto(int socket_fd, bool is_server, const std::string& algo_name, bool use_generated_params)
{
    use_generated_params = false;
    std::cout << "[DEBUG] starting DH and crypto, algo: " << algo_name << std::endl;
    
    I_feistel_chipher* cipher = nullptr;
    
    try
    {
        DHParameters params;
        
        if (use_generated_params)
        {
            params = generate_parameters(256);
        }
        else
        {
            params.p = bigint("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
            params.g = 2;
        }
        
        bigint p = params.p;
        bigint g = params.g;
        
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<unsigned long long> dist(2, 1000000);
        bigint private_key = dist(gen);
        
        std::cout << "[DEBUG] private key generated (random)" << std::endl;
        
        bigint public_key = mod_pow(g, private_key, p);
        
        if (is_server)
        {
            std::cout << "[DEBUG] server: sending public key" << std::endl;
            send_bigint(socket_fd, public_key);
            send_bigint(socket_fd, p);
            send_bigint(socket_fd, g);
            
            bigint other_public = receive_bigint(socket_fd);
            if (other_public == -1)
            {
                std::cout << "[ERROR] failed to receive other public key" << std::endl;
                return;
            }
            
            bigint shared_secret = mod_pow(other_public, private_key, p);
            std::cout << "[DEBUG] shared secret generated" << std::endl;
            
            std::size_t k_size = 0;
            std::size_t b_size = 0;
            
            if (algo_name == "des")
            {
                cipher = new des_algorithm();
                k_size = 8;
                b_size = 8;
            }
            else if (algo_name == "aes")
            {
                cipher = new rijndael(128);
                k_size = 16;
                b_size = 16;
            }
            else if (algo_name == "mars")
            {
                cipher = new mars_cipher();
                k_size = 32;
                b_size = 16;
            }
            else
            {
                std::cout << "[ERROR] unknown algorithm: " << algo_name << std::endl;
                return;
            }
            
            std::vector<std::uint8_t> key = extract_key_bytes(shared_secret, k_size);
            cipher->setup_round_keys(key);
            std::cout << "[DEBUG] encryption key prepared" << std::endl;
            
            std::vector<std::uint8_t> plain_text = generate_random_bytes(b_size);
            
            std::cout << "plain text: ";
            for (auto b : plain_text)
            {
                std::cout << std::hex << (int)b << " ";
            }
            std::cout << std::dec << std::endl;
            
            std::vector<std::uint8_t> cipher_text = cipher->encrypt_block(plain_text);
            std::cout << "[DEBUG] message encrypted, sending..." << std::endl;
            send_buffer(socket_fd, cipher_text);
        }
        else
        {
            std::cout << "[DEBUG] client: receiving public key" << std::endl;
            bigint other_public = receive_bigint(socket_fd);
            if (other_public == -1)
            {
                std::cout << "[ERROR] failed to receive other public key" << std::endl;
                return;
            }
            
            bigint p = receive_bigint(socket_fd);
            if (p == -1)
            {
                std::cout << "[ERROR] failed to receive p" << std::endl;
                return;
            }
            
            bigint g = receive_bigint(socket_fd);
            if (g == -1)
            {
                std::cout << "[ERROR] failed to receive g" << std::endl;
                return;
            }
            
            send_bigint(socket_fd, public_key);
            
            bigint shared_secret = mod_pow(other_public, private_key, p);
            std::cout << "[DEBUG] shared secret generated" << std::endl;
            
            std::size_t k_size = 0;
            
            if (algo_name == "des")
            {
                cipher = new des_algorithm();
                k_size = 8;
            }
            else if (algo_name == "aes")
            {
                cipher = new rijndael(128);
                k_size = 16;
            }
            else if (algo_name == "mars")
            {
                cipher = new mars_cipher();
                k_size = 32;
            }
            else
            {
                std::cout << "[ERROR] unknown algorithm: " << algo_name << std::endl;
                return;
            }
            
            std::vector<std::uint8_t> key = extract_key_bytes(shared_secret, k_size);
            cipher->setup_round_keys(key);
            std::cout << "[DEBUG] decryption key prepared" << std::endl;
            
            std::cout << "[DEBUG] waiting for encrypted message..." << std::endl;
            std::vector<std::uint8_t> cipher_text = receive_buffer(socket_fd);
            if (cipher_text.empty())
            {
                std::cout << "[ERROR] received empty buffer" << std::endl;
                delete cipher;
                return;
            }
            
            std::vector<std::uint8_t> decrypted = cipher->decrypt_block(cipher_text);
            std::cout << "[DEBUG] message decrypted" << std::endl;
            
            std::cout << "decrypted data: ";
            for (auto b : decrypted)
            {
                std::cout << std::hex << (int)b << " ";
            }
            std::cout << std::dec << std::endl;
        }
        
        delete cipher;
    }
    catch (const std::exception& e)
    {
        std::cout << "[ERROR] exception in crypto: " << e.what() << std::endl;
        if (cipher) delete cipher;
    }
    catch (...)
    {
        std::cout << "[ERROR] unknown exception in crypto" << std::endl;
        if (cipher) delete cipher;
    }
}

int main(int argc, char* argv[])
{
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN);
    
    if (argc < 3)
    {
        std::cout << "usage: " << argv[0] << " <server|client> <des|aes|mars> [--fixed-params]" << std::endl;
        return 1;
    }
    
    std::string mode = argv[1];
    std::string algo = argv[2];
    bool use_generated_params = true;
    
    if (argc >= 4 && std::string(argv[3]) == "--fixed-params")
    {
        use_generated_params = false;
        std::cout << "[DEBUG] using fixed DH parameters (legacy mode)" << std::endl;
    }
    
    int ret = 0;
    
    if (mode == "server")
    {
        std::cout << "[DEBUG] server started on port 9000" << std::endl;
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_fd < 0)
        {
            std::cout << "[ERROR] failed to create socket" << std::endl;
            return 1;
        }
        
        int opt = 1;
        setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(9000);
        
        if (bind(server_socket_fd, (sockaddr*)&addr, sizeof(addr)) < 0)
        {
            std::cout << "[ERROR] failed to bind socket" << std::endl;
            close(server_socket_fd);
            server_socket_fd = -1;
            return 1;
        }
        
        if (listen(server_socket_fd, 1) < 0)
        {
            std::cout << "[ERROR] failed to listen on socket" << std::endl;
            close(server_socket_fd);
            server_socket_fd = -1;
            return 1;
        }
        
        std::cout << "[DEBUG] waiting for connection..." << std::endl;
        
        client_socket_fd = accept(server_socket_fd, nullptr, nullptr);
        if (client_socket_fd < 0)
        {
            std::cout << "[ERROR] failed to accept connection" << std::endl;
            close(server_socket_fd);
            server_socket_fd = -1;
            return 1;
        }
        
        std::cout << "[DEBUG] client connected" << std::endl;
        perform_dh_and_crypto(client_socket_fd, true, algo, use_generated_params);
        
        close(client_socket_fd);
        client_socket_fd = -1;
        close(server_socket_fd);
        server_socket_fd = -1;
        std::cout << "[DEBUG] server finished" << std::endl;
    }
    else if (mode == "client")
    {
        std::cout << "[DEBUG] client started, connecting to 127.0.0.1:9000" << std::endl;
        client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket_fd < 0)
        {
            std::cout << "[ERROR] failed to create socket" << std::endl;
            return 1;
        }
        
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(9000);
        if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0)
        {
            std::cout << "[ERROR] invalid address" << std::endl;
            close(client_socket_fd);
            client_socket_fd = -1;
            return 1;
        }
        
        if (connect(client_socket_fd, (sockaddr*)&addr, sizeof(addr)) < 0)
        {
            std::cout << "[ERROR] failed to connect to server" << std::endl;
            close(client_socket_fd);
            client_socket_fd = -1;
            return 1;
        }
        
        std::cout << "[DEBUG] connected to server" << std::endl;
        perform_dh_and_crypto(client_socket_fd, false, algo, use_generated_params);
        
        close(client_socket_fd);
        client_socket_fd = -1;
        std::cout << "[DEBUG] client finished" << std::endl;
    }
    else
    {
        std::cout << "[ERROR] unknown mode: " << mode << std::endl;
        std::cout << "usage: " << argv[0] << " <server|client> <des|aes|mars> [--fixed-params]" << std::endl;
        return 1;
    }
    
    return ret;
}