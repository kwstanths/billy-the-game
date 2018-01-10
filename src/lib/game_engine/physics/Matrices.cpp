#include "Matrices.hpp"

using namespace glm;

namespace game_engine {

    mat4 GetModelMatrix(vec3 translate_vec, vec3 scale_vec, GLfloat rotate_deg, vec3 rotate_axis_vec) {
        return GetTranslateMatrix(translate_vec) * GetScaleMatrix(scale_vec) * GetRotateMatrix(rotate_deg, rotate_axis_vec);
    }

    mat4 GetTranslateMatrix(vec3 translate_vec) {
        mat4 transform_translate = translate(mat4(), translate_vec);
        return transform_translate * mat4(1.0f);
    }

    mat4 GetScaleMatrix(vec3 scale_vec) {
        mat4 transform_scale = scale(mat4(), scale_vec);
        return transform_scale * mat4(1.0f);
    }

    mat4 GetRotateMatrix(GLfloat rotate_deg, vec3 rotate_axis_vec) {
        mat4 transform_rotate = rotate(mat4(), rotate_deg, rotate_axis_vec);
        return transform_rotate * mat4(1.0f);
    }

}

