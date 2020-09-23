#ifndef __GraphicsTypes_hpp__
#define __GraphicsTypes_hpp__

#include <string>

#include <glm/glm.hpp>

#include "game_engine/math/Types.hpp"

namespace game_engine {
namespace graphics {

#define GAME_ENGINE_TEXTURE_TYPE_EMPTY              -1
#define GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP        0
#define GAME_ENGINE_TEXTURE_TYPE_SPECULAR_MAP       1
#define GAME_ENGINE_TEXTURE_TYPE_NORMAL_MAP         2
#define GAME_ENGINE_TEXTURE_TYPE_DISPLACEMENT_MAP   3

#define GAME_ENGINE_MATERIAL_EMERALD        0
#define GAME_ENGINE_MATERIAL_JADE           1
#define GAME_ENGINE_MATERIAL_OBSIDIAN       2
#define GAME_ENGINE_MATERIAL_PEARL          3
#define GAME_ENGINE_MATERIAL_RUBY           4
#define GAME_ENGINE_MATERIAL_TURQUOISE      5
#define GAME_ENGINE_MATERIAL_BRASS          6
#define GAME_ENGINE_MATERIAL_BRONZE         7
#define GAME_ENGINE_MATERIAL_CHROME         8
#define GAME_ENGINE_MATERIAL_COPPER         9
#define GAME_ENGINE_MATERIAL_GOLD           10
#define GAME_ENGINE_MATERIAL_SILVER         11
#define GAME_ENGINE_MATERIAL_BLACK_PLASTIC  12
#define GAME_ENGINE_MATERIAL_CYAN_PLASTIC   13
#define GAME_ENGINE_MATERIAL_GREEN_PLASTIC  14
#define GAME_ENGINE_MATERIAL_RED_PLASTIC    15
#define GAME_ENGINE_MATERIAL_WHITE_PLASTIC  16
#define GAME_ENGINE_MATERIAL_YELLOW_PLASTIC 17
#define GAME_ENGINE_MATERIAL_BLACK_RUBBER   18
#define GAME_ENGINE_MATERIAL_CYAN_RUBBER    19
#define GAME_ENGINE_MATERIAL_GREEN_RUBBER   20
#define GAME_ENGINE_MATERIAL_RED_RUBBER     21
#define GAME_ENGINE_MATERIAL_WHITE_RUBBER   22
#define GAME_ENGINE_MATERIAL_YELLOW_RUBBER  23

    /**
        A struct to hold vertex data
    */
    struct Vertex_t {
        Vertex_t() {};
        Vertex_t(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv) : position_(pos), normal_(norm), uv_(uv) {};
        glm::vec3 position_;
        glm::vec3 normal_;
        glm::vec2 uv_;
    };

    struct Texture_t {
        std::string path_;
        int type_;
        Texture_t() {};
        Texture_t(std::string path, int type) : path_(path), type_(type) {};
    };


    /**
        Holds the necessary values for the light attenuation aglrothim
    */
    struct Attenuation_t {
        Real_t constant_;
        Real_t linear_;
        Real_t quadratic_;

        /**
            Attenuation values are big enough
        */
        Attenuation_t() {
            constant_ = 1.0f;
            linear_ = 0.0014f;
            quadratic_ = 0.000007f;
        }

        /**
            Create a custom attenuation struct
        */
        Attenuation_t(Real_t c, Real_t l, Real_t q) : constant_(c), linear_(l), quadratic_(q) {};
    };

    /**
    
    */
    struct Wave_t {
        glm::vec3 direction_;
        Real_t wavelength_;
        Real_t amplitude_;
        Wave_t(glm::vec3 direction, Real_t wavelength, Real_t amplitude) : direction_(direction), wavelength_(wavelength), amplitude_(amplitude) {};
    };

}
}
#endif