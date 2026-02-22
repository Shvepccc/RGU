#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <csignal>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/select.h>
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
#define POLL_INTERVAL_MS 100  // Интервал проверки в миллисекундах

struct SharedBlock
{
    size_t key_size;
    size_t data_size;
    int operation;
    int status;     // 0 - свободна, 1 - занята, 2 - готов ответ, -1 - ошибка
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
    int pipefd[2];  // Для прерывания ожидания
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
    
    // Прерываем все ожидающие потоки
    for (auto& session : sessions)
    {
        if (session.pipefd[1] != -1)
        {
            write(session.pipefd[1], "x", 1);  // Отправляем сигнал прерывания
        }
    }
}

// Функция для ожидания семафора с возможностью прерывания (для macOS)
bool wait_sem_with_interrupt(sem_t* sem, int interrupt_fd, int timeout_ms = -1)
{
    fd_set set;
    struct timeval tv, *ptv = nullptr;
    
    if (timeout_ms >= 0)
    {
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        ptv = &tv;
    }
    
    while (running)
    {
        FD_ZERO(&set);
        FD_SET(interrupt_fd, &set);
        
        // Проверяем семафор неблокирующе
        if (sem_trywait(sem) == 0)
        {
            return true;  // Получили семафор
        }
        
        if (errno != EAGAIN)
        {
            return false;  // Реальная ошибка
        }
        
        // Ждём либо прерывания, либо таймаута
        select(interrupt_fd + 1, &set, nullptr, nullptr, ptv);
        
        if (FD_ISSET(interrupt_fd, &set))
        {
            char buf;
            read(interrupt_fd, &buf, 1);  // Очищаем канал
            if (!running)
            {
                return false;  // Нас прервали для завершения
            }
        }
        else if (ptv != nullptr)
        {
            // Таймаут истёк
            return false;
        }
    }
    
    return false;
}

void session_worker(int id, SharedBlock* block, sem_t* sem_req, sem_t* sem_resp, int interrupt_fd)
{
    std::cout << "[SERVER] Worker for session " << id << " started\n";
    
    while (running)
    {
        // Ожидаем запрос от клиента с возможностью прерывания
        if (wait_sem_with_interrupt(sem_req, interrupt_fd, 1000))  // Таймаут 1 сек
        {
            std::cout << "[SERVER] Session " << id << " processing request\n";
            
            // Проверяем корректность данных
            if (block->key_size + block->data_size > MAX_DATA)
            {
                std::cerr << "[SERVER] Session " << id << ": data too large\n";
                block->status = -1;
                sem_post(sem_resp);
                continue;
            }
            
            try
            {
                // Извлекаем ключ и данные
                std::vector<uint8_t> key(block->buffer, block->buffer + block->key_size);
                std::vector<uint8_t> data(block->buffer + block->key_size, 
                                          block->buffer + block->key_size + block->data_size);
                
                // Выполняем RC4
                RC4 rc4(key);
                rc4.process(data);
                
                // Копируем результат обратно (на место исходных данных)
                memcpy(block->buffer, data.data(), data.size());
                
                // Отмечаем успешное завершение
                block->status = 2;
                std::cout << "[SERVER] Session " << id << " completed, size=" << data.size() << "\n";
            }
            catch (const std::exception& e)
            {
                std::cerr << "[SERVER] Session " << id << " error: " << e.what() << "\n";
                block->status = -1;
            }
            
            // Сигнализируем клиенту о готовности
            sem_post(sem_resp);
        }
        // Если wait вернул false из-за прерывания или таймаута, просто продолжаем цикл
    }
    
    std::cout << "[SERVER] Worker for session " << id << " stopped\n";
}

bool create_session(int id)
{
    std::string shm_name = "/rc4_session_" + std::to_string(id);
    std::string sem_req_name = "/rc4_req_" + std::to_string(id);
    std::string sem_resp_name = "/rc4_resp_" + std::to_string(id);

    // Удаляем старые объекты (если остались)
    shm_unlink(shm_name.c_str());
    sem_unlink(sem_req_name.c_str());
    sem_unlink(sem_resp_name.c_str());

    // Создаём разделяемую память
    int shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        std::cerr << "[SERVER] shm_open failed for session " << id << ": " << strerror(errno) << "\n";
        return false;
    }

    // Устанавливаем размер
    if (ftruncate(shm_fd, sizeof(SharedBlock)) == -1)
    {
        std::cerr << "[SERVER] ftruncate failed for session " << id << ": " << strerror(errno) << "\n";
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }

    // Отображаем память
    void* ptr = mmap(nullptr, sizeof(SharedBlock), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        std::cerr << "[SERVER] mmap failed for session " << id << ": " << strerror(errno) << "\n";
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }

    // Инициализируем структуру
    SharedBlock* block = static_cast<SharedBlock*>(ptr);
    memset(block, 0, sizeof(SharedBlock));
    block->status = 0;  // Свободна

    // Создаём семафоры
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

    // Создаём pipe для прерывания ожидания
    SessionContext ctx;
    ctx.id = id;
    ctx.shm_fd = shm_fd;
    ctx.block = block;
    ctx.sem_req = sem_req;
    ctx.sem_resp = sem_resp;
    ctx.running = true;
    
    if (pipe(ctx.pipefd) == -1)
    {
        std::cerr << "[SERVER] pipe failed for session " << id << ": " << strerror(errno) << "\n";
        sem_close(sem_req);
        sem_close(sem_resp);
        sem_unlink(sem_req_name.c_str());
        sem_unlink(sem_resp_name.c_str());
        munmap(ptr, sizeof(SharedBlock));
        close(shm_fd);
        shm_unlink(shm_name.c_str());
        return false;
    }
    
    // Запускаем рабочий поток
    ctx.worker = std::thread(session_worker, id, block, sem_req, sem_resp, ctx.pipefd[0]);
    
    sessions.push_back(std::move(ctx));
    
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
            // Отправляем сигнал прерывания потоку
            if (it->pipefd[1] != -1)
            {
                write(it->pipefd[1], "x", 1);
            }
            
            it->running = false;
            if (it->worker.joinable())
            {
                it->worker.join();
            }
            
            // Закрываем файловые дескрипторы
            if (it->pipefd[0] != -1) close(it->pipefd[0]);
            if (it->pipefd[1] != -1) close(it->pipefd[1]);
            
            // Закрываем и удаляем объекты
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
        if (session.block->status == 1)  // Занята обработкой
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

    // Создаём начальные сессии
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

    // Основной цикл мониторинга
    while (running)
    {
        scale_sessions_if_needed();
        
        // Проверяем завершившиеся сессии (со статусом 2 или -1)
        for (auto& session : sessions)
        {
            if (session.block->status == 2 || session.block->status == -1)
            {
                session.block->status = 0;  // Освобождаем для следующего запроса
            }
        }
        
        sleep(1);
    }

    // Корректное завершение
    std::cout << "[SERVER] Stopping all sessions...\n";
    
    // Копируем ID сессий для уничтожения
    std::vector<int> ids;
    for (const auto& session : sessions)
    {
        ids.push_back(session.id);
    }
    
    for (int id : ids)
    {
        destroy_session(id);
    }
    
    std::cout << "[SERVER] Shutdown complete\n";
    return 0;
}