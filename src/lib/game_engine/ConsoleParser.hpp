#ifndef __ConsoleParser_hpp__
#define __ConsoleParser_hpp__

#include <string>
#include <thread>
#include <vector>
#include <unordered_map>

#include "debug_tools/Timestamp.hpp"

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
#define COMMAND_CA_RESET 9
#define COMMAND_CA_SPAWN 10
#define COMMAND_CA_INTEGRATION 11
#define COMMAND_CA_BOUNCING 12
#define COMMAND_CA_FRICTION 13
#define COMMAND_CA_ELASTICITY 14
#define COMMAND_CA_DAMPING 15
#define COMMAND_CA_WORLD 16
#define COMMAND_CA_DRAW_PARTICLES 17
#define COMMAND_CA_DRAW_SPRING_LINES 18
#define COMMAND_CA_DRAW_SPRING_TRIANGLES 19
#define COMMAND_CA_PAUSE 20

    /* The order between these two is very important! */
    static const std::vector<std::string> commands = { "ssao_radius", 
        "ssao_samples", 
        "ssao_blur", 
        "ssao_blur_size", 
        "ssao_intensity", 
        "ssao_bias",
        "ssao_draw_ssao", 
        "ssao_separable",
        "shadows",

        "reset",
        "spawn",
        "integration",
        "bouncing",
        "friction",
        "elasticity",
        "damping",
        "world",
        "draw_particles",
        "draw_lines",
        "draw_triangles",
        "pause"
    };

    struct ConsoleCommand {
        uint64_t timestamp_ = 0;
        int type_ = -1;
        float arg_1_;
        float arg_2_;

        bool operator==(const ConsoleCommand& other) {
            return other.timestamp_ == timestamp_;
        }

        bool operator!=(const ConsoleCommand& other) {
            return other.timestamp_ != timestamp_;
        }
    };

    class ConsoleParser {
    public:
        static ConsoleParser & GetInstance() {
            static ConsoleParser instance;
            return instance;
        }

        ~ConsoleParser();

        ConsoleCommand GetLastCommand();

        void ClearLastCommand();

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