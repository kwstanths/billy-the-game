#ifndef __FrameRateRegulator_hpp__
#define __FrameRateRegulator_hpp__

#ifdef _WIN32
#include <Windows.h>
#endif

#include <vector>

#include <glfw3.h>

namespace game_engine {

    /**
        Class to measure frame deltas, and try to maintain a certain frame rate
    */
    class FrameRateRegulator {
    public:

        /**
            Initialize the class. Private values initialization
            @param frame_rate The frame rate to try to maintain
        */
        void Init(size_t frame_rate = 0, size_t frame_averages = 5);

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
            Signals the end of the frame, calulates the frame time
            @return 0=OK, -1=Not initialized
        */
        int FrameEnd();

        /**
            Get the frame time in seconds using a running average, First frame will be 1/frame_rate requested
            @return The difference between frame_end_time - frame_start_time
        */
        float GetDelta();

        /**
            Writes the frame rate in the console. Just pass the frame time in milliseconds
            @param frame_rate_ms The frame rate in milliseconds
        */
        void DisplayFPS(double frame_rate_ms);

    private:
        bool is_inited_;
        double frame_start_time_, frame_stop_time_;
        double frame_time_required_;

        /* Holds the frame times for the latest frames. Used to calulcate the running average */
        std::vector<double> deltas_;

#ifdef _WIN32
        HANDLE timer_;
#endif

    };

}



#endif