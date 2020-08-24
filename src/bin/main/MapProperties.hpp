#ifndef __MapProperties_hpp__
#define __MapProperties_hpp__

#include <string>

#include "game_engine/utility/HashTable.hpp"

/* 
    Holds the properties of the map layers. If a tile has collision,
    if a tile is a point light, etc.
*/
class MapProperties {
public:
    MapProperties();

    void ReadMap(std::string map_file);

    bool HasCollision(int tile_id, std::string& collision_string);

    bool IsLight(int tile_id, float& intensity);

private:
    game_engine::utility::HashTable<int, std::string> * tile_collisions_;
    game_engine::utility::HashTable<int, float> * tile_lights_;

};

#endif