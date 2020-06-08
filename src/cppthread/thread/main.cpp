#include <atomic>
#include <thread>
#include <future>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

int thread_worker(int x) {
    for (size_t i = 0; i < 2; i++) {
        std::cout << x << std::endl;
        sleep(1);
    }
    return x + 1;
}

void run_async() {
    std::promise<int> res;
    std::future<int> future = std::async(std::launch::deferred, thread_worker, 15);
    sleep(2);
    std::cout << future.get() << std::endl;
}

void run_mutex() {
    std::mutex m;
    size_t counter = 0;
    auto worker = [&] {
        for (size_t i = 0; i < 1024; i++) {
            std::lock_guard<std::mutex> guard{m};
            counter += 1;
        }
    };
    std::thread th1{worker}, th2{worker};
    th1.join();
    th2.join();
    std::cout << counter << std::endl;
}

class SpinLock {
public:
    void lock() {
        while (!try_lock()) {
            asm("":::"memory");
        }
    }

    bool try_lock() {
        int target = 0;
        return locked_.compare_exchange_weak(target, 1, std::memory_order_acquire);
    }

    void release() {
        locked_.store(0, std::memory_order_release);
    }

private:
    std::atomic<int> locked_ = 0;
};

void run_atomic() {
    SpinLock s;
    s.lock();
    std::cout << s.try_lock() << std::endl;
    std::cout << s.try_lock() << std::endl;
    s.release();
    std::cout << s.try_lock() << std::endl;
    std::cout << s.try_lock() << std::endl;
    std::thread th([&] { s.release(); });
    s.lock();
    th.join();
}

class Semaphore {
public:
    explicit Semaphore(size_t initial) : counter_(initial) {}
    Semaphore() : Semaphore(0) { }

    void acquire() {
        std::unique_lock guard{mutex_};
        while (counter_ == 0) {
            cond_.wait(guard);
        }
        counter_--;
    }

    void release() {
        std::lock_guard guard{mutex_};
        counter_++;
        cond_.notify_one();
    }

private:
    size_t counter_ = 0;
    std::mutex mutex_;
    std::condition_variable cond_;
};

void run_semaphore() {
    Semaphore sem{2};
    sem.acquire();
    sem.acquire();

    std::thread th{[&] { sleep(2); sem.release(); }};

    sem.acquire();
    std::cout << "ok" << std::endl;
    sem.acquire();
    th.join();
}



int main() {
    run_semaphore();
    return 0;
}
