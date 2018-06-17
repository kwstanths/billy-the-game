#ifndef __FIFOWorker_hpp__
#define __FIFOWorker_hpp__

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "Task.hpp"


namespace game_engine {

namespace utility {

    class FIFOWorker {
    public:
        /**
            Does nothing explicit. See Init()
        */
        FIFOWorker();

        /**
            Calls Stop()
        */
        ~FIFOWorker();

        /**
            Initializes a single thread to work on scheduled items. See Schedule()
        */
        void Init();

        /**
            Schedules a new work item to be processed in time with FIFO ordering. Should
            return immidiately
            @param func A functor, i.e a lambda expression, a function, a struct with the
                operator() defined e.t.c
        */
        void Schedule(std::function<void()> func);

        /**
            Stops the worker gracefully, i.e. executes all remaining tasks, and returns
        */
        void Stop();

        /**
        
        */
        void BusyWaitAll();

    private:
        std::thread running_thread_;
        std::mutex lock_;
        std::condition_variable condition_;
        std::queue<Task> tasks_;
        bool run_;
        bool is_inited_;
        bool executing_;

        void run();
    };

}
}

#endif
