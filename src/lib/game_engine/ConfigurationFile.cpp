#include "ConfigurationFile.hpp"

#include <fstream>
#include <string>
#include <vector>

#include "utility/BasicFunctions.hpp"

namespace game_engine {

    bool ConfigurationFile::DoSSAO() {
        return ssao_;
    }

    int ConfigurationFile::GetRenderingMethod() {
        return rendering_method;
    }

    ConfigurationFile::ConfigurationFile() {
        /* Read configuration file */
        std::string file_name = "config.txt";

        std::ifstream infile(file_name);

        std::string line;
        while (std::getline(infile, line)) {
            std::vector<std::string> line_split = utility::split(line, "=");
            if (line_split.size() < 1) continue;

            if (line_split[0] == "rendering_method") rendering_method = std::stoi(line_split[1]);
            if (line_split[0] == "ssao") ssao_ = static_cast<bool>(std::stoi(line_split[1]));
        }
    }

}

