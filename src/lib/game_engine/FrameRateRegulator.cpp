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

    void FrameRateRegulator::Init(size_t frame_rate, size_t frame_averages) {

        if (frame_rate == 0) frame_time_required_ = 0;
        else frame_time_required_ = 1.0 / (1.0 * frame_rate);

        /* Initialize the deltas vector with the frame time needed in seconds */
        deltas_ = std::vector<double>(frame_averages, frame_time_required_);

#ifdef _WIN32
        /* 
            Don't know if it does anything, but in order to maintain an accurate constant 
            frame rate, we need to be able to sleep and wake a certain amount of time accurately.
            This means that the scheduler should not waste our time, and shcdule us cpu the time 
            instantly
        */
        if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
            dt::ConsoleInfoL(dt::WARNING, "Failed to set priority", "error", GetLastError());

        /* 
            False means it's a synchronization timer, the timer will be sending a signal at a constant 
            period of time 
        */
        timer_ = CreateWaitableTimer(NULL, FALSE, NULL);
        if (timer_ == NULL) return;

        /* Signal the timer every frame_time_required seconds, starting after the timer_interval time */
        LARGE_INTEGER timer_interval;
        timer_interval.QuadPart = -(10000LL * frame_time_required_ * 1000);
        SetWaitableTimer(timer_, &timer_interval, frame_time_required_ * 1000, NULL, 0, NULL);
#endif

        is_inited_ = true;
    }

    void FrameRateRegulator::Destroy() {
        
        CodeReminder("TODO Cancel, stop, reset the timer");

        is_inited_ = false;
    }

    int  FrameRateRegulator::FrameStart() {
        if (!is_inited_) return -1;

        frame_start_time_ = glfwGetTime();

        return 0;
    }

    int FrameRateRegulator::FrameEnd() {
        if (!is_inited_) return -1;

#ifdef _WIN32
        /*
            Wait for the timer signal. INFINITE means that if the frame took more time than frame_time_required
            then we will wait the next signal, meaning that we will lose a frame
        */
        WaitForSingleObject(timer_, /*(frame_time_required_ - actual_delta) * 1000*/ INFINITE);
#elif __linux__
        usleep(1000 * 1000 * (frame_time_required_ - actual_delta));
#endif
        /* Push the previous frame times, and store the new one */
        for (size_t i = 0; i < deltas_.size() - 1; i++) deltas_[i] = deltas_[i + 1];
        deltas_[deltas_.size() - 1] = glfwGetTime() - frame_start_time_;
        
        DisplayFPS(deltas_[deltas_.size() - 1] * 1000.0);
            
        return 0;
    }

    float FrameRateRegulator::GetDelta() {
        /* Calculate a running average of the frame times */
        double avg = 0.0;
        for (size_t i = 0; i < deltas_.size(); i++) {
            avg += deltas_[i] / ((double)deltas_.size());
        }
        return avg;
    }

    void FrameRateRegulator::DisplayFPS(double frame_time_ms) {

        /* That's so nasty */
        static double start;
        static unsigned int frames;

        start += frame_time_ms;
        frames++;
        if (start >= 1000.0f) {

            std::cout << "FPS: " << frames << std::endl;

            start = 0.0;
            frames = 0;
        }

    }

}
