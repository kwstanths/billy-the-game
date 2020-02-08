#include "ConsoleParser.hpp"

#include <iostream>
#include <future>
#include <chrono>

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    ConsoleParser::ConsoleParser() {

        for (size_t i = 0; i < commands.size(); i++)
            command_types_[commands[i]] = i;

        abort_ = false;
        parser_thread_ = std::thread(&ConsoleParser::Parse, this);
    }

    ConsoleParser::~ConsoleParser() {

        abort_ = true;

        /* TODO How do you unblock a blocked thread waiting for IO? Just request the user to type something 4Head */
        dt::Console("Press Enter to exit...");
        
        parser_thread_.join();
    }

    ConsoleCommand ConsoleParser::GetLastCommand() {
        return last_command_;
    }

    void ConsoleParser::ClearLastCommand() {
        last_command_ = ConsoleCommand();
    }

    void ConsoleParser::Parse() {

        while (!abort_) {
            std::cout << " >> ";
            std::getline(std::cin, line_);
            
            std::vector<std::string> tokens = split(line_, " ");
            ProcessCommand(tokens);

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return;
    }

    std::vector<std::string> ConsoleParser::split(const std::string & str, const std::string & delim) {
        std::vector<std::string> tokens;
        size_t prev = 0, pos = 0;
        do
        {
            pos = str.find(delim, prev);
            if (pos == std::string::npos) pos = str.length();
            std::string token = str.substr(prev, pos - prev);
            if (!token.empty()) tokens.push_back(token);
            prev = pos + delim.length();
        } while (pos < str.length() && prev < str.length());
        return tokens;
    }

    void ConsoleParser::ProcessCommand(std::vector<std::string>& tokens) {

        if (tokens.size() == 0) return;

        std::unordered_map<std::string, int>::iterator itr = command_types_.find(tokens[0]);
        if (itr == command_types_.end()) return;

        int type = itr->second;
        
        last_command_.timestamp_ = debug_tools::GetMSecondsSinceEpoch();
        switch (type)
        {
        case COMMAND_SSAO_RADIUS:
        case COMMAND_SSAO_SAMPLES:
        case COMMAND_SSAO_BLUR:
        case COMMAND_SSAO_BLUR_SIZE:
        case COMMAND_SSAO_INTENSITY:
        case COMMAND_SSAO_DRAW_SSAO:
        case COMMAND_SSAO_SEPARABLE:
        case COMMAND_SSAO_BIAS:
        case COMMAND_SHADOW_MAPPING:
        case COMMAND_CA_SPAWN:
        case COMMAND_CA_INTEGRATION:
        case COMMAND_CA_BOUNCING:
        case COMMAND_CA_FRICTION:
        case COMMAND_CA_ELASTICITY:
        case COMMAND_CA_DAMPING:
        case COMMAND_CA_WORLD:
        case COMMAND_CA_DRAW_PARTICLES:
        case COMMAND_CA_DRAW_SPRING_LINES:
        case COMMAND_CA_DRAW_SPRING_TRIANGLES:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = type;

            break;
        }
        case COMMAND_CA_RESET:
        case COMMAND_CA_PAUSE:
        {
            last_command_.type_ = type;
            break;
        }
        default:
            break;
        }
        
    }

}