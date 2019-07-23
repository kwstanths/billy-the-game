#ifndef __MapProperties_hpp__
#define __MapProperties_hpp__

#include <string>

#include "game_engine/utility/HashTable.hpp"

class MapProperties {
public:
    MapProperties();

    void ReadMap(std::string map_file);

    bool HasCollision(std::string name);

    bool IsLight(std::string name);

private:
    game_engine::utility::HashTable<std::string, int> * tile_collisions_;
    game_engine::utility::HashTable<std::string, int> * tile_lights_;

};

#endif