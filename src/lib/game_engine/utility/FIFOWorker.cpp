#include "FIFOWorker.hpp"

namespace game_engine {

    FIFOWorker::FIFOWorker() {
        is_inited_ = false;
    }

    FIFOWorker::~FIFOWorker() {
        Stop();
    }

    void FIFOWorker::Init() {
        if (is_inited_) return;

        run_ = true;
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

    void FIFOWorker::run() {
        is_inited_ = true;
        while (1) {
            {   /* Wait for task to be scheduled */
                std::unique_lock<std::mutex> l(lock_);
                condition_.wait(l, [&]() { return !tasks_.empty() || !run_; });
            }

            if (!run_ && tasks_.empty()) break;

            if (!tasks_.empty()) {
                /* Pop front task... */
                Task task = tasks_.front();
                tasks_.pop();
                /* ... and run it's () operator */
                task();
            }
        }
    }

}