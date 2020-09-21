#include "FileSystem.hpp"

#include <fstream>
#include <vector>

#include "game_engine/core/ConfigurationFile.hpp"
#include "game_engine/utility/BasicFunctions.hpp"

namespace game_engine {
    
    std::string FileSystem::GetDirectoryAssets()
    {
        return directory_assets_;
    }
    
    std::string FileSystem::GetDirectoryShaders()
    {
        return directory_shaders_;
    }

    FileSystem::FileSystem() {
        /* Read configuration file */
        std::string file_name = "config.txt";

        std::ifstream infile(file_name);

        std::string line;
        while (std::getline(infile, line)) {
            std::vector<std::string> line_split = utility::split(line, "=");
            if (line_split.size() < 1) continue;

            if (line_split[0] == "directory_assets") directory_assets_ = line_split[1];
            if (line_split[0] == "directory_shaders") directory_shaders_ = line_split[1];
        }
    }

}

