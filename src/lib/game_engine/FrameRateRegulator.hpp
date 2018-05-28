#ifndef __FrameRateRegulator_hpp__
#define __FrameRateRegulator_hpp__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <vector>

#include <glfw3.h>

namespace game_engine {

    /**
        Class to measure frame deltas, and regulate to certain frame rate
    */
    class FrameRateRegulator {
    public:

        /**
            Initialize the class. Private values initialization
            @param frame_rate The frame rate to try to maintain
        */
        void Init(size_t frame_rate = 0);

        /**
            Destroy the object. Private values clearing
        */
        void Destroy();

        /**
            Stores the frame start time 
            @return 0=OK, -1=Not initialized
        */
        int FrameStart();

        /**
            Stores the frame end time, Sleeps if needed to achieve the required frame rate, calculates the delta
            @return 0=OK, -1=Not initialized
        */
        int FrameEnd();

        /**
            Get the last frame's delta time in seconds, First frame will be 1/frame_rate requested
            @return The difference between frame_end_time - frame_start_time
        */
        float GetDelta();

    private:
        bool is_inited_;
        double frame_start_time_, frame_stop_time_, delta_;
        double frame_time_required_;

#ifdef _WIN32
        HANDLE timer_;
#endif

    };

}



#endif