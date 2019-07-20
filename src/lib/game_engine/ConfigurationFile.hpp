#ifndef __ConfigurationFile_hpp__
#define __ConfigurationFile_hpp__

#include <iostream>


namespace game_engine {

    class ConfigurationFile {
    public:

        static ConfigurationFile &instance() {
            static ConfigurationFile instance;

            return instance;
        }

        bool DoSSAO();

        int GetRenderingMethod();

    private:
        ConfigurationFile();

        int rendering_method = 0;
        bool ssao_ = false;
    };

}

#endif

