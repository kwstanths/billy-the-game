#ifndef __ConfigurationFile_hpp__
#define __ConfigurationFile_hpp__

#include <iostream>


namespace game_engine {

    class ConfigurationFile {
    public:

        static ConfigurationFile &GetInstance() {
            static ConfigurationFile instance;

            return instance;
        }

        bool DoSSAO();

        int GetRenderingMethod();

        bool UseVisibleWindow();

    private:
        ConfigurationFile();

        int rendering_method = 0;
        bool ssao_ = false;
        bool visible_window_ = false;
    };

}

#endif

