#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>    

#include "game_engine/math/Vector.hpp"
#include "game_engine/utility/BasicFunctions.hpp"

using namespace game_engine::math;
using namespace game_engine;

/** 
    Holds a unique tile in the map
*/
struct Tile {
    std::vector<Vector3D> vertices_;
    std::vector<Vector2D> uv_;
    std::vector<Vector3D> triangles_;
};

/**
    Function used to export individual tiles as separate meshes (objects) into a single wavefront .obj file
*/
void ExportObjFile(std::ofstream& obj_file, std::string file_name, size_t index, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
    obj_file << "o Plane." + std::to_string(index) + "\n";
    obj_file << "v -0.500000 -0.500000 0.000000\n";
    obj_file << "v 0.500000 -0.500000 0.000000\n";
    obj_file << "v -0.500000 0.500000 0.000000\n";
    obj_file << "v 0.500000 0.500000 0.000000\n";
    obj_file << "vt " << u1 << " " << v1 << "\n";
    obj_file << "vt " << u2 << " " << v2 << "\n";
    obj_file << "vt " << u3 << " " << v3 << "\n";
    obj_file << "vt " << u4 << " " << v4 << "\n";
    obj_file << "vn 0.0000 0.0000 1.0000\n";
    obj_file << "usemtl " + file_name + "\n";
    obj_file << "s off\n";
    /* In a wavefront obj file, the face is indixed like this: v/vt/vn. As a result, with the following all faces point to the same first 4 vertices */
    obj_file << "f 1/" + std::to_string(3 + index * 4) + "/1 2/" + std::to_string(1 + index * 4) + "/1 3/" + std::to_string(2 + index * 4) + "/1\n";
    obj_file << "f 2/" + std::to_string(1 + index * 4) + "/1 4/" + std::to_string(4 + index * 4) + "/1 3/" + std::to_string(2 + index * 4) + "/1\n";
}

/**
    Function used to create a unique Tile
*/
Tile CreateTile(float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
    
    Tile temp;
    temp.vertices_.push_back(Vector3D({ -0.5, -0.5, 0 }));
    temp.vertices_.push_back(Vector3D({  0.5, -0.5, 0 }));
    temp.vertices_.push_back(Vector3D({ -0.5,  0.5, 0 }));
    temp.vertices_.push_back(Vector3D({  0.5,  0.5, 0 }));
    temp.uv_.push_back(Vector2D({ u3, v3 }));
    temp.uv_.push_back(Vector2D({ u1, v1 }));
    temp.uv_.push_back(Vector2D({ u2, v2 }));
    temp.uv_.push_back(Vector2D({ u4, v4 }));
    temp.triangles_.push_back(Vector3D({ 1, 2, 3 }));
    temp.triangles_.push_back(Vector3D({ 2, 4, 3 }));
    return temp;
}

/**
    Reads a map file, and generates tiles for every tile in the map. Adds the vertices, uvs, and triangles into data structures, to be exported to a single .obj
*/
int PackMapLayer(std::string name, float z, std::unordered_map<int, Tile> tiles, std::vector<Vector3D>& vertices, std::vector<Vector2D>& uvs, std::vector<Vector3D>& triangles, int index) {

    dt::Console("Packing map " + name + " layer: " + std::to_string(z));

    std::vector<std::vector<std::string>> map;

    std::string line;
    std::ifstream myfile(name);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            map.push_back(utility::split(line, ","));
        }
        myfile.close();
    }
    else {
        dt::Console(dt::WARNING, "Can't open map file: " + name);
        return 0;
    }

    /* Map sizes */
    int map_height = 154;
    int map_width = 178;
    float map_width_2 = ((float)map_width) / 2.0f;
    float map_height_2 = ((float)map_height) / 2.0f;
    
    int tile_index = index;
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            Real_t x = j - map_width_2;
            Real_t y = map_height_2 - i;

            /* If its a valid tile */
            int id = std::stoi(map[i][j]);
            if (id == -1) continue;
            
            if (tiles.find(id) == tiles.end()) continue;
            
            /* Get the unique tile that this map position corresponds to */
            Tile tile = tiles[id];
            
            /* Get the position of the tile in the map */
            Vector3D tile_position({ x, y, z });
            
            /* Transform the vertices of the tile, to reflect the position in the map */
            for (size_t i = 0; i < tile.vertices_.size(); i++) {
                vertices.push_back(tile.vertices_[i] + tile_position);
            }

            for (size_t i = 0; i < tile.uv_.size(); i++) {
                uvs.push_back(tile.uv_[i]);
            }

            /* Add the index for each triangle, since all tiles are exported into a single mesh */
            for (size_t i = 0; i < tile.triangles_.size(); i++) {
                triangles.push_back(tile.triangles_[i] + Vector3D(tile_index));
            }
            
            tile_index += tile.vertices_.size();
        }
    }
    return tile_index;
}


int main(int argc, char ** argv) {

    /* Create a single .obj file, with multiple meshes for all the available unique tiles. Maybe we have to spawn the individually on the fly */
    std::ofstream obj_file;
    obj_file.open("roguelikeSheet_transparent.obj");
    obj_file << "mtllib materials/roguelikeSheet_transparent.mtl\n";

    size_t horizontal_resolution = 968;
    size_t vertical_resolution = 526;

    size_t asset_horizontal_pixel_width = 16;
    size_t asset_vertical_pixel_width = 16;
    size_t horizontal_margin = 1;
    size_t vertical_margin = 1;

    size_t nrows = (vertical_resolution + 1) / (asset_vertical_pixel_width + 1);
    size_t ncols = (horizontal_resolution + 1) / (asset_horizontal_pixel_width + 1);
    
    float asset_width_h = (float)asset_horizontal_pixel_width / (float)horizontal_resolution;
    float asset_width_v = (float)asset_vertical_pixel_width / (float)vertical_resolution;

    float asset_border_margin_h = 0.2f / (float)horizontal_resolution;
    float asset_border_margin_v = 0.2f / (float)vertical_resolution;

    /* Create the unique tiles */
    size_t asset_index = 0;
    std::unordered_map<int, Tile> tiles;
    for (size_t i = 0; i < nrows; i++) {
        for (size_t j = 0; j < ncols; j++) {
            
            size_t pixel_position_h = (asset_horizontal_pixel_width + horizontal_margin) * j;
            size_t pixel_position_v = (asset_vertical_pixel_width + vertical_margin) * i;

            float u = (float ) pixel_position_h / (float) horizontal_resolution;
            float v = 1.0f - (float) pixel_position_v / (float) vertical_resolution;

            float u1 = u + asset_border_margin_h;
            float v1 = v - asset_border_margin_v;
            float u2 = u + asset_width_h - asset_border_margin_h;
            float v2 = v - asset_border_margin_v;
            float u3 = u + asset_border_margin_h;
            float v3 = v - asset_width_v + asset_border_margin_v;
            float u4 = u + asset_width_h - asset_border_margin_h;
            float v4 = v - asset_width_v + asset_border_margin_v;
            /*std::cout << u1 << " " << v1 << std::endl;
            std::cout << u2 << " " << v2 << std::endl;
            std::cout << u3 << " " << v3 << std::endl;
            std::cout << u4 << " " << v4 << std::endl;*/
            
            /* Export tiles into a single .obj object */
            ExportObjFile(obj_file, "roguelikeSheet_transparent", asset_index, u4, v4, u1, v1, u3, v3, u2, v2);

            /* Get a copy of the tile data, to use it for packing the map */
            Tile tile = CreateTile(u4, v4, u1, v1, u3, v3, u2, v2);
            tiles.insert(std::make_pair(asset_index, tile));

            asset_index++;
        }
    }

    /* Write the material file for the single tiles mesh atlas, with all the unique tiles */
    std::ofstream mtl_file;
    mtl_file.open("roguelikeSheet_transparent.mtl");
    mtl_file << "newmtl roguelikeSheet_transparent \n";
    mtl_file << "Ns 2.000000\n";
    mtl_file << "Ka 0.000000 0.000000 0.000000\n";
    mtl_file << "Kd 0.000000 0.000000 0.000000\n";
    mtl_file << "Ks 0.000000 0.000000 0.000000\n";
    mtl_file << "Ke 0.000000 0.000000 0.000000\n";
    mtl_file << "Ni 1.000000\n";
    mtl_file << "d 1.000000\n";
    mtl_file << "illum 0\n";
    mtl_file << "map_Kd textures/roguelikeSheet_transparent.png\n";
    mtl_file << "map_Ks textures/spec_map_empty.png\n";
    mtl_file << "\n";

    /* 
        Prepare the map files 
        The map files spawn all the tiles inside the map, in their respective positions with a single .obj file with all the geometry for all tiles inside the map
    */
    dt::Console("Preparing the map... ");
    std::ofstream map_obj_file;
    map_obj_file.open("static_map.obj");
    map_obj_file << "mtllib materials/static_map.mtl\n";
    map_obj_file << "o static_map\n";

    /* Hold all vertices, uvs, and triangles for all tiles inside the map */
    std::vector<Vector3D> vertices;
    std::vector<Vector2D> uvs;
    std::vector<Vector3D> triangles;
    int tile_index = 0;
    /* Read maps */
    tile_index = PackMapLayer("billy_map_Tile Layer 1.csv", 0.0f, tiles, vertices, uvs, triangles, tile_index);
    tile_index = PackMapLayer("billy_map_Tile Layer 2.csv", 0.03f, tiles, vertices, uvs, triangles, tile_index);

    /* Write geometry into the map object file */
    dt::CustomPrint(std::cout, "Writing " + std::to_string(vertices.size()) + " vertices\n");
    for (size_t i = 0; i < vertices.size(); i++) {
        map_obj_file << "v " << vertices[i].x() << " " << vertices[i].y() << " " << vertices[i].z() << "\n";
    }

    dt::CustomPrint(std::cout, "Writing " + std::to_string(uvs.size()) + " uv coordinates\n");
    for (size_t i = 0; i < uvs.size(); i++) {
        map_obj_file << "vt " << uvs[i].x() << " " << uvs[i].y() << "\n";
    }

    map_obj_file << "vn 0.0000 0.0000 1.0000\n";
    map_obj_file << "usemtl static_map_mtl\n";
    map_obj_file << "s off\n";

    dt::Console("Writing triangles... ");
    for (size_t i = 0; i < triangles.size(); i++) {
        map_obj_file << "f " << triangles[i].x()  << "/" << triangles[i].x() << "/1 " << triangles[i].y() << "/" << triangles[i].y() << "/1 " << triangles[i].z() << "/" << triangles[i].z() << "/1\n";
    }
    map_obj_file.close();

    /* Write the material file for the map object */
    std::ofstream map_mtl_file;
    map_mtl_file.open("static_map.mtl");
    map_mtl_file << "newmtl static_map_mtl\n";
    map_mtl_file << "Ns 2.000000\n";
    map_mtl_file << "Ka 0.000000 0.000000 0.000000\n";
    map_mtl_file << "Kd 0.000000 0.000000 0.000000\n";
    map_mtl_file << "Ks 0.000000 0.000000 0.000000\n";
    map_mtl_file << "Ke 0.000000 0.000000 0.000000\n";
    map_mtl_file << "Ni 1.000000\n";
    map_mtl_file << "d 1.000000\n";
    map_mtl_file << "illum 0\n";
    map_mtl_file << "map_Kd textures/roguelikeSheet_transparent.png\n";
    map_mtl_file << "map_Ks textures/spec_map_empty.png\n";
    map_mtl_file << "\n";

#ifdef _WIN32
    system("pause");
#endif

}
