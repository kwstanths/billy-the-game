#ifndef __Matrices_hpp__
#define __Matrices_hpp__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Types.hpp"

namespace game_engine {
namespace math {

    /**
        Get a model matrix
        @param translate_vec A vector for translation
        @param scale_vec A vector for scaling
        @param rotate_deg A rotation given in degrees
        @param rotate_axis_vec The axis for rotation
        @return The Model matrix
     */
    glm::mat4 GetModelMatrix(Real_t translate_x, Real_t translate_y, Real_t translate_z,
        Real_t scale_x, Real_t scale_y, Real_t scale_z,
        Real_t rotate_deg, Real_t rotate_axis_x, Real_t rotate_axis_y, Real_t rotate_axis_z);

    /**
        Get a translation matrix
        @param translate_x The translation amount in x direction
        @param translate_y The translation amount in y direction
        @param translate_z The translation amount in z direction
        @return The translation matrix
     */
    glm::mat4 GetTranslateMatrix(Real_t translate_x, Real_t translate_y, Real_t translate_z);

    /**
        Get a scale matrix
        @param scale_x The amount of scaling in x direction
        @param scale_y The amount of scaling in y direction
        @param scale_z The amount of scaling in z direction
        @return The scale matrix
     */
    glm::mat4 GetScaleMatrix(Real_t scale_x, Real_t scale_y, Real_t scale_z);

    /**
        Get a rotation matrix
        @param rotate_deg A rotation given in radians
        @param rotate_axis_x The x coordinate of the axis to rotate around
        @param rotate_axis_y The y coordinate of the axis to rotate around
        @param rotate_axis_z The z coordinate of the axis to rotate around
        @return The rotation matrix
     */
    glm::mat4 GetRotateMatrix(Real_t rotate_deg, Real_t rotate_axis_x, Real_t rotate_axis_y, Real_t rotate_axis_z);

    /**
        Multiply two 4x4 matrices, res = a * b
        @param[out] res Resulting matrix
        @param a Input matrix a
        @param b Input matrix b
    */
    void MultMat(Real_t *res, Real_t *a, Real_t *b);
}
}


#endif
