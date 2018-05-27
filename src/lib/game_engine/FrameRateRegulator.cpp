#include "FrameRateRegulator.hpp"

#ifdef _WIN32
#include "Windows.h"
#elif __linux__
#include <unistd.h> 
#endif

#include <iostream>
#include <chrono>
#include <thread>

#include "debug_tools/CodeReminder.hpp"
#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    void FrameRateRegulator::Init(size_t frame_rate) {

        if (frame_rate == 0) frame_time_required_ = 0;
        else frame_time_required_ = 1.0 / (1.0 * frame_rate);

        delta_ = frame_time_required_;

        is_inited_ = true;
    }

    void FrameRateRegulator::Destroy() {
        
        is_inited_ = false;
    }

    int  FrameRateRegulator::FrameStart() {
        if (!is_inited_) return -1;

        frame_start_time_ = glfwGetTime();
        return 0;
    }

    int FrameRateRegulator::FrameEnd() {
        if (!is_inited_) return -1;

        frame_stop_time_ = glfwGetTime();

        double actual_delta = frame_stop_time_ - frame_start_time_;
        
        //dt::ConsoleInfo("acutal delta", actual_delta * 1000.0f);
        
        if (actual_delta < frame_time_required_) {
            int msec_sleep = (frame_time_required_ - actual_delta) * 1000.0f ;
            //dt::ConsoleInfo("Sleep time", msec_sleep);
#ifdef _WIN32
            while(glfwGetTime()- frame_stop_time_ < frame_time_required_)
                std::this_thread::sleep_for(std::chrono::microseconds(1000));
#elif __linux__
            usleep(1000 * 1000 * (frame_time_required_ - actual_delta));
#endif
            delta_ = glfwGetTime() - frame_start_time_;
        } else {
            delta_ = actual_delta;
        }

        return 0;
    }

    float FrameRateRegulator::GetDelta() {
        return delta_;
    }

}
