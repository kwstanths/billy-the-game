#ifndef __FileSystem_hpp__
#define __FileSystem_hpp__

#include <string>

namespace game_engine {

    class FileSystem {
    public:
        static FileSystem &GetInstance() {
            static FileSystem instance;

            return instance;
        }

        std::string GetDirectoryAssets();

        std::string GetDirectoryShaders();

    private:
        FileSystem();

        std::string directory_assets_;
        std::string directory_shaders_;
    };

}
#endif