#ifndef __Task_hpp__
#define __Task_hpp__

#include <functional>


namespace game_engine {

namespace utility {

    class Task {
    private:
        std::function<void()> func_;

    public:
        Task() {};

        Task(std::function<void()> func) : func_(func) {};

        void operator()() {
            func_();
        }

    };

}
}


#endif
