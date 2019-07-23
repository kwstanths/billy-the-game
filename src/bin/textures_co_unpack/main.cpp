#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void CreateFiles(std::string title, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4) {
    std::ofstream obj_file;
    obj_file.open(title + ".obj");
    obj_file << "mtllib materials/" + title + ".mtl\n";
    obj_file << "o Plane\n";
    obj_file << "v -0.500000 -0.500000 0.000000\n";
    obj_file << "v 0.500000 -0.500000 0.000000\n";
    obj_file << "v -0.500000 0.500000 0.000000\n";
    obj_file << "v 0.500000 0.500000 0.000000\n";

    obj_file << "vt " << u1 << " " << v1 << "\n";
    obj_file << "vt " << u2 << " " << v2 << "\n";
    obj_file << "vt " << u3 << " " << v3 << "\n";
    obj_file << "vt " << u4 << " " << v4 << "\n";
    obj_file << "vn 0.0000 0.0000 1.0000\n";
    obj_file << "usemtl " + title << "\n";
    obj_file << "s off\n";
    obj_file << "f 2/1/1 3/2/1 1/3/1\n";
    obj_file << "f 2/1/1 4/4/1 3/2/1\n";
    obj_file.close();

    std::ofstream mtl_file;
    mtl_file.open(title + ".mtl");
    mtl_file << "newmtl " + title + "\n";
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
    mtl_file.close();
}


int main(int argc, char ** argv) {

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

    size_t asset_index = 0;
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
            std::cout << u1 << " " << v1 << std::endl;
            std::cout << u2 << " " << v2 << std::endl;
            std::cout << u3 << " " << v3 << std::endl;
            std::cout << u4 << " " << v4 << std::endl;
            CreateFiles("roguelikeSheet_transparent_" + std::to_string(asset_index), u4, v4, u1, v1, u3, v3, u2, v2);
            asset_index++;
        }
    }


    float u = 0.0f;
    float v = 1.0f;

    

#ifdef _WIN32
    system("pause");
#endif

}