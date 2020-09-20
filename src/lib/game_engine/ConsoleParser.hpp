#ifndef __ConsoleParser_hpp__
#define __ConsoleParser_hpp__

#include <string>
#include <thread>
#include <vector>
#include <unordered_map>

namespace game_engine {

#define COMMAND_SSAO_RADIUS 0
#define COMMAND_SSAO_SAMPLES 1
#define COMMAND_SSAO_BLUR 2
#define COMMAND_SSAO_BLUR_SIZE 3
#define COMMAND_SSAO_INTENSITY 4
#define COMMAND_SSAO_BIAS 5
#define COMMAND_SSAO_DRAW_SSAO 6
#define COMMAND_SSAO_SEPARABLE 7
#define COMMAND_SHADOW_MAPPING 8
#define COMMAND_WIREFRAME 9
#define COMMAND_TERRAIN_NORMALS 10
#define COMMAND_CONSTANT_TESSELLATION 11

    /* The order between these two is very important! */
    static const std::vector<std::string> commands = { 
        "ssao_radius", 
        "ssao_samples", 
        "ssao_blur", 
        "ssao_blur_size", 
        "ssao_intensity", 
        "ssao_bias",
        "ssao_draw_ssao", 
        "ssao_separable",

        "shadows",
        "wireframe",
        "draw_terrain_normals",
        "constant_tess"
    };

    struct ConsoleCommand {
        int type_ = -1;
        float arg_1_;
        float arg_2_;
    };

    class ConsoleParser {
    public:
        static ConsoleParser & GetInstance() {
            static ConsoleParser instance;
            return instance;
        }

        ~ConsoleParser();

        ConsoleCommand GetLastCommand();

    private:
        std::thread parser_thread_;
        bool abort_;
        std::unordered_map<std::string, int> command_types_;

        std::string line_;
        ConsoleCommand last_command_;

        void Parse();

        std::vector<std::string> split(const std::string& str, const std::string& delim);

        void ProcessCommand(std::vector<std::string>& tokens);

        ConsoleParser();
    };

}

#endif