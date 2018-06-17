#include "FIFOWorker.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

namespace utility {

    FIFOWorker::FIFOWorker() {
        is_inited_ = false;
    }

    FIFOWorker::~FIFOWorker() {
        Stop();
    }

    void FIFOWorker::Init() {
        if (is_inited_) return;

        run_ = true;
        executing_ = false;
        running_thread_ = std::thread(&FIFOWorker::run, this);

        /* Wait for the thread to spawn and start the run() function */
        while (!is_inited_);

        return;
    }

    void FIFOWorker::Schedule(std::function<void()> func) {
        {
            std::unique_lock<std::mutex> l(lock_);
            tasks_.push(Task(func));
        }
        condition_.notify_one();
    }

    void FIFOWorker::Stop() {
        if (!is_inited_) return;

        run_ = false;
        condition_.notify_one();
        running_thread_.join();

        is_inited_ = false;
    }

    void FIFOWorker::BusyWaitAll() {
        while (executing_ || !tasks_.empty());
    }

    void FIFOWorker::run() {
        is_inited_ = true;
        while (1) {
            Task task;
            {   /* Wait for task to be scheduled */
                std::unique_lock<std::mutex> l(lock_);
                condition_.wait(l, [&]() { return !tasks_.empty() || !run_; });

                if (!run_ && tasks_.empty()) break;
                
                executing_ = true;

                /* Pop front task... */
                task = tasks_.front();
                tasks_.pop();
            }
            /* ... and run it's () operator */
            task();
            executing_ = false;
        }
    }

}
}
