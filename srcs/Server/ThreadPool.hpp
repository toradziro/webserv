#pragma once

#include <common_inc.h>
#include <ClientHandler.hpp>
#include <Log.hpp>

class ThreadPool {
public:
    ThreadPool(int numThreads) {
        m_threads.reserve(numThreads);
        for(int i = 0; i < numThreads; ++i) {
            m_threads.emplace_back(&ThreadPool::run, this);
        }
    }

    void addTask(ClientHandler clientHandler) {
        std::lock_guard<std::mutex> queueLock(m_queueMutex);
        m_taskQueue.emplace(std::move(clientHandler));
        m_queueConditionVar.notify_one();
    }

    ~ThreadPool() {
        m_quit = true;
        for(size_t i = 0; i < m_threads.size(); ++i) {
            m_queueConditionVar.notify_all();
            m_threads[i].join();
    }
}

private:
    void run() {
        int currThreadId = m_threadId;
        ++m_threadId;
        while(!m_quit) {
            // need to lock thread untill new tasks
            std::unique_lock<std::mutex> lock(m_queueMutex);
            // if we have tasks wee take it
            // lambda that has access to this and returns bool
            m_queueConditionVar.wait(lock, [this]()->bool { 
                return !m_taskQueue.empty() || m_quit; 
            });

            if(!m_taskQueue.empty()) {
                ClientHandler clientHandler = std::move(m_taskQueue.front());
                m_taskQueue.pop();
                clientHandler.setThreadId(currThreadId);
                try {
                    clientHandler.HandleRequest();
                } catch(CException* e) {
                    LogMessage(e->getDescription());
                    delete e;
                }
            }
        }
    }

    std::vector<std::thread>            m_threads;
    std::queue<ClientHandler>           m_taskQueue;
    std::mutex                          m_queueMutex;
    std::condition_variable             m_queueConditionVar;
    std::atomic<bool>                   m_quit{ false };
    int                                 m_threadId{ 1 };
};
