#include "Matrices.hpp"

using namespace glm;

namespace game_engine {
namespace math {

    mat4 GetModelMatrix(Real_t translate_x, Real_t translate_y, Real_t translate_z,
        Real_t scale_x, Real_t scale_y, Real_t scale_z,
        Real_t rotate_deg, Real_t rotate_axis_x, Real_t rotate_axis_y, Real_t rotate_axis_z)
    {
        return GetTranslateMatrix(translate_x, translate_y, translate_z)
            * GetScaleMatrix(scale_x, scale_y, scale_z)
            * GetRotateMatrix(rotate_deg, rotate_axis_x, rotate_axis_y, rotate_axis_z);
    }

    mat4 GetTranslateMatrix(Real_t translate_x, Real_t translate_y, Real_t translate_z) {
        mat4 transform_translate = translate(mat4(), glm::vec3(translate_x, translate_y, translate_z));
        return transform_translate * mat4(1.0f);
    }

    mat4 GetScaleMatrix(Real_t scale_x, Real_t scale_y, Real_t scale_z) {
        mat4 transform_scale = scale(mat4(), glm::vec3(scale_x, scale_y, scale_z));
        return transform_scale * mat4(1.0f);
    }

    mat4 GetRotateMatrix(Real_t rotate_deg, Real_t rotate_axis_x, Real_t rotate_axis_y, Real_t rotate_axis_z) {
        mat4 transform_rotate = rotate<game_engine::Real_t, glm::highp>(mat4(), rotate_deg, glm::vec3(rotate_axis_x, rotate_axis_y, rotate_axis_z));
        return transform_rotate * mat4(1.0f);
    }

    void MultMat(Real_t *res, Real_t *a, Real_t *b) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                res[i * 4 + j] = 0.0;
                for (int k = 0; k < 4; k++) {
                    res[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
                }
            }
        }
    }

}
}

