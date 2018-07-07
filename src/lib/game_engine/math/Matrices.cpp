#include "Matrices.hpp"

using namespace glm;

namespace game_engine {

    mat4 GetModelMatrix(float translate_x, float translate_y, float translate_z,
        float scale_x, float scale_y, float scale_z,
        float rotate_deg, float rotate_axis_x, float rotate_axis_y, float rotate_axis_z)
    {
        return GetTranslateMatrix(translate_x, translate_y, translate_z) 
            * GetScaleMatrix(scale_x, scale_y, scale_z) 
            * GetRotateMatrix(rotate_deg, rotate_axis_x, rotate_axis_y, rotate_axis_z);
    }

    mat4 GetTranslateMatrix(float translate_x, float translate_y, float translate_z) {
        mat4 transform_translate = translate(mat4(), glm::vec3(translate_x, translate_y, translate_z));
        return transform_translate * mat4(1.0f);
    }

    mat4 GetScaleMatrix(float scale_x, float scale_y, float scale_z) {
        mat4 transform_scale = scale(mat4(), glm::vec3(scale_x, scale_y, scale_z));
        return transform_scale * mat4(1.0f);
    }

    mat4 GetRotateMatrix(float rotate_deg, float rotate_axis_x, float rotate_axis_y, float rotate_axis_z) {
        mat4 transform_rotate = rotate(mat4(), rotate_deg, glm::vec3(rotate_axis_x, rotate_axis_y, rotate_axis_z));
        return transform_rotate * mat4(1.0f);
    }

}

