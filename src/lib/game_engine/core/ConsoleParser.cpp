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
        switch (type)
        {
        case COMMAND_SSAO_RADIUS:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_RADIUS;

            break;
        }
        case COMMAND_SSAO_SAMPLES:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_SAMPLES;

            break;
        }
        case COMMAND_SSAO_BLUR:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_BLUR;
            break;
        }
        case COMMAND_SSAO_BLUR_SIZE:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_BLUR_SIZE;
            break;
        }
        case COMMAND_SSAO_INTENSITY:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_INTENSITY;
            break;
        }
        case COMMAND_SSAO_DRAW_SSAO:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_DRAW_SSAO;
            break;
        }
        case COMMAND_SSAO_SEPARABLE:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_SEPARABLE;
            break;
        }
        case COMMAND_SSAO_BIAS:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SSAO_BIAS;
            break;
        }
        case COMMAND_SHADOW_MAPPING:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stof(tokens[1]);
            last_command_.type_ = COMMAND_SHADOW_MAPPING;
            break;
        }
        case COMMAND_WIREFRAME:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stoi(tokens[1]);
            last_command_.type_ = COMMAND_WIREFRAME;
            break;
        }
        case COMMAND_TERRAIN_NORMALS:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stoi(tokens[1]);
            last_command_.type_ = COMMAND_TERRAIN_NORMALS;
            break;
        }
        case COMMAND_CONSTANT_TESSELLATION:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stoi(tokens[1]);
            last_command_.type_ = COMMAND_CONSTANT_TESSELLATION;
            break;
        }
        case COMMAND_CAMERA_SPEED:
        {
            if (tokens.size() <= 1) return;
            last_command_.arg_1_ = std::stoi(tokens[1]);
            last_command_.type_ = COMMAND_CAMERA_SPEED;
            break;
        }
        default:
            break;
        }
        
    }

}