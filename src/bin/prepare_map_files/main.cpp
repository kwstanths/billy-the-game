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
    temp.vertices_.push_back(Vector3D(-0.5, -0.5, 0));
    temp.vertices_.push_back(Vector3D( 0.5, -0.5, 0));
    temp.vertices_.push_back(Vector3D(-0.5,  0.5, 0));
    temp.vertices_.push_back(Vector3D( 0.5,  0.5, 0));
    temp.uv_.push_back(Vector2D(u3, v3));
    temp.uv_.push_back(Vector2D(u1, v1));
    temp.uv_.push_back(Vector2D(u2, v2));
    temp.uv_.push_back(Vector2D(u4, v4));
    temp.triangles_.push_back(Vector3D(1, 2, 3));
    temp.triangles_.push_back(Vector3D(2, 4, 3));
    return temp;
}

/* Hold the map width and height */
static int MAP_HEIGHT;
static int MAP_WIDTH;

/* Used for hashing the packed map structure */
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 * MAP_HEIGHT + h2;
    }
};
class PackedMap {
public:
    PackedMap() {

        /* Read map layers, init map structures */
        {
            std::string name = "billy_map_Tile Layer 1.csv";
            std::string line;
            std::ifstream myfile(name);
            if (myfile.is_open()) {
                while (getline(myfile, line)) {
                    map_1_.push_back(utility::split(line, ","));
                }
                myfile.close();
            } else {
                dt::Console(dt::WARNING, "Can't open map file: " + name);
            }
        }

        {
            std::string name = "billy_map_Tile Layer 2.csv";
            std::string line;
            std::ifstream myfile(name);
            if (myfile.is_open()) {
                while (getline(myfile, line)) {
                    map_2_.push_back(utility::split(line, ","));
                }
                myfile.close();
            } else {
                dt::Console(dt::WARNING, "Can't open map file: " + name);
            }
        }

        MAP_HEIGHT  = map_1_.size();
        MAP_WIDTH = map_1_[0].size();

        map_width_2 = ((float)MAP_HEIGHT) / 2.0f;
        map_height_2 = ((float)MAP_WIDTH) / 2.0f;
    }

    /* Pack all the objects in the world map inside a region, into a single object, given a position and a width and a height, that represents a region in the world
    
    */
    void PackMapLayer(std::unordered_map<int, Tile>& tiles, int pos_i, size_t i_width2, int pos_j, size_t j_height2) {
        int tile_index = 0;
        /* Read map layers */
        tile_index = PackMapLayer(map_1_, 0.0f, tiles, tile_index, pos_i, i_width2, pos_j, j_height2);
        tile_index = PackMapLayer(map_2_, 0.03f, tiles, tile_index, pos_i, i_width2, pos_j, j_height2);
    }

    void ExportMapFiles() {
        /* Prepare a small file to hold the correspondence between packed tile regions and positions in the world */
        dt::Console("Preparing the packed map files... ");
        std::ofstream packed_map_tiles_file;
        packed_map_tiles_file.open("packed_map_tiles.txt");
        /* Create a single .obj file to hold all tile regions */
        std::ofstream map_obj_file;
        map_obj_file.open("static_map.obj");
        map_obj_file << "mtllib materials/static_map.mtl\n";

        int vertex_index = 0;
        int uv_index = 0;
        int map_index = 0;
        for (auto itr = packed_map_.begin(); itr != packed_map_.end(); ++itr) {
            TileRegion& region = itr->second;

            if (region.vertices_.size() == 0) continue;
            /* 
                Write the data for a single packed region, naming static_map.NUMBER
            */
            map_obj_file << "o static_map." + std::to_string(map_index) +"\n";
            
            /* Write geometry */
            for (size_t i = 0; i < region.vertices_.size(); i++) {
                map_obj_file << "v " << region.vertices_[i].x() << " " << region.vertices_[i].y() << " " << region.vertices_[i].z() << "\n";
            }
            
            for (size_t i = 0; i < region.uv_.size(); i++) {
                map_obj_file << "vt " << region.uv_[i].x() << " " << region.uv_[i].y() << "\n";
            }
            
            map_obj_file << "vn 0.0000 0.0000 1.0000\n";
            map_obj_file << "usemtl static_map_mtl\n";
            map_obj_file << "s off\n";
            
            /* Write triangles, reflect the fact that everything is written in a singile file, thus, vertex_index, uv_index */
            for (size_t i = 0; i < region.triangles_.size(); i++) {
                // v/vt/vn
                map_obj_file << "f " << region.triangles_[i].x() + vertex_index  << "/" << region.triangles_[i].x() + uv_index << "/1 " 
                    << region.triangles_[i].y() + vertex_index << "/" << region.triangles_[i].y() + uv_index << "/1 " 
                    << region.triangles_[i].z() + vertex_index << "/" << region.triangles_[i].z() + uv_index << "/1\n";
            }

            vertex_index += region.vertices_.size();
            uv_index += region.uv_.size();

            /* Write the correspondence in the map .txt */
            packed_map_tiles_file << itr->first.first << " " << itr->first.second << " static_map_" + std::to_string(map_index) + "\n";
            map_index++;
        }

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
    }

private:
    /* holds the .csv map layers */
    std::vector<std::vector<std::string>> map_1_;
    std::vector<std::vector<std::string>> map_2_;
    float map_width_2;
    float map_height_2;

    typedef Tile TileRegion;
    /* Holds the packed region tiles */
    std::unordered_map<std::pair<int, int>, TileRegion, pair_hash> packed_map_;

    /**
        Reads a map file, and generates tiles for every tile in the map. Adds the vertices, uvs, and triangles into data structures, to be exported to a single .obj
    */
    int PackMapLayer(std::vector<std::vector<std::string>>& map, float z, std::unordered_map<int, Tile>& tiles, int index, int pos_i, int i_width2, int pos_j, int j_height2) {

        /* Get the position of the big packed region inside the world map */
        /* TODO: There is a shift happening on the tiles of the map for some reason... */
        int center_x = pos_j - map_width_2 - 12;
        int center_y = map_height_2 - pos_i - 12;

        /* Get the tile region from the packed map */
        TileRegion& region = packed_map_[std::make_pair(center_x, center_y)];

        /* Pack all the unique tiles inside that region, into the single packed tile map */
        int tile_index = index;
        for (int i = -i_width2; i < i_width2; i++) {
            for (int j = -j_height2; j < j_height2; j++) {

                /* If index is valid */
                if (pos_i + i >= MAP_HEIGHT) continue;
                if (pos_j + j >= MAP_WIDTH) continue;
                int id = std::stoi(map[pos_i + i][pos_j + j]);
                /* and its a valid tile */
                if (id == -1) continue;
                if (tiles.find(id) == tiles.end()) continue;

                /* Get the unique tile that should be placed in that point in the world */
                Tile tile = tiles[id];

                /* Get the position of the tile in the map, invert the coordinates */
                Vector3D tile_position({ (Real_t)(j), (Real_t)(-i), z });

                /* Transform the vertices of the tile, to reflect the position in the map */
                for (size_t v = 0; v < tile.vertices_.size(); v++) {
                    region.vertices_.push_back(tile.vertices_[v] + tile_position);
                }

                for (size_t uv = 0; uv < tile.uv_.size(); uv++) {
                    region.uv_.push_back(tile.uv_[uv]);
                }

                /* Add the index for each triangle, since all tiles are exported into a single packed mesh */
                for (size_t t = 0; t < tile.triangles_.size(); t++) {
                    region.triangles_.push_back(tile.triangles_[t] + Vector3D(tile_index));
                }

                tile_index += tile.vertices_.size();
            }
        }
        return tile_index;
    }
};




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

    /* Init packed map */
    PackedMap packed_map;

    /* Pack regions of the world, with a width and a height of 20, into packed meshes */
    size_t strive = 20;
    size_t strive2 = 10;

    for (int i = strive2; i < MAP_HEIGHT; i+= strive) {
        for (int j = strive2; j < MAP_WIDTH; j+=strive) {
            packed_map.PackMapLayer(tiles, i, strive2, j, strive2);
        }
    }

    packed_map.ExportMapFiles();

#ifdef _WIN32
    system("pause");
#endif

}
