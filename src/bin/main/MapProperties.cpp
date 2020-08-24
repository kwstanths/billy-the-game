#include "MapProperties.hpp"

#include <iostream>
#include <fstream>

#include "game_engine/utility/BasicFunctions.hpp"

namespace utl = game_engine::utility;

MapProperties::MapProperties() {
    tile_collisions_ = new utl::HashTable<int, std::string>(256);
    tile_lights_ = new utl::HashTable<int, float>(256);
}

void MapProperties::ReadMap(std::string map_file) {

    std::vector<std::string> temp = utl::split(map_file, ".");
    std::string tileset_name = temp[0];

    std::vector<std::string> lines;
    
    {
        std::string line;
        std::ifstream myfile(map_file);
        if (myfile.is_open()) {
            while (getline(myfile, line)) {
                lines.push_back(line);
            }
            myfile.close();
        }
    }

    size_t line = 3;
    while (line != lines.size() - 1) {
        /* Get the tile id */
        size_t quote_1 = lines[line].find("\"");
        size_t quote_2 = lines[line].find("\"",quote_1+1);
        int tile = std::stoi(lines[line].substr(quote_1 + 1, quote_2 - quote_1 - 1));
        line += 2;

        /* Get the first property */
        quote_1 = lines[line].find("\"");
        quote_2 = lines[line].find("\"", quote_1 + 1);
        std::string prop = lines[line].substr(quote_1 + 1, quote_2 - quote_1 - 1);
        if (prop == "collision") {
            quote_1 = quote_2 + 8;
            quote_2 = lines[line].find("\"", quote_1 + 1);
            std::string collision_value = lines[line].substr(quote_1 + 1, quote_2 - quote_1 - 1);
            tile_collisions_->Insert(tile, collision_value);
        } else if (prop == "light") {
            quote_1 = quote_2 + 21;
            quote_2 = lines[line].find("\"", quote_1 + 1);
            std::string light_value = lines[line].substr(quote_1 + 1, quote_2 - quote_1 - 1);
            tile_lights_->Insert(tile, std::stof(light_value));
        }

        line++;
        quote_1 = lines[line].find("\"");
        if (quote_1 == std::string::npos) {
            line += 2;
            continue;
        }
        quote_2 = lines[line].find("\"", quote_1 + 1);
        prop= lines[line].substr(quote_1 + 1, quote_2 - quote_1 - 1);
        tile_lights_->Insert(tile, 1);

        line += 3;
    }

}

bool MapProperties::HasCollision(int tile_id, std::string& collision_string) {
    utl::HashTable<int, std::string>::iterator itr = tile_collisions_->Find(tile_id);
    if (itr != tile_collisions_->end()) {
        collision_string = itr.GetValue();
        return true;
    };
    return false;
}

bool MapProperties::IsLight(int tile_id, float& intensity) {
    utl::HashTable<int, float>::iterator itr = tile_lights_->Find(tile_id);
    if (itr != tile_lights_->end()) {
        intensity = itr.GetValue();
        return true;
    }
    return false;
}
