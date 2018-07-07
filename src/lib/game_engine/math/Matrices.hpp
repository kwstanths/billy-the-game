#ifndef __Matrices_hpp__
#define __Matrices_hpp__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace game_engine {

    /**
        Get a model matrix
        @param translate_vec A vector for translation
        @param scale_vec A vector for scaling
        @param rotate_deg A rotation given in degrees
        @param rotate_axis_vec The axis for rotation
        @return The Model matrix
     */
    glm::mat4 GetModelMatrix(float translate_x, float translate_y, float translate_z, 
        float scale_x, float scale_y, float scale_z,
        float rotate_deg, float rotate_axis_x, float rotate_axis_y, float rotate_axis_z);

    /**
        Get a translation matrix
        @param translate_x The translation amount in x direction
        @param translate_y The translation amount in y direction
        @param translate_z The translation amount in z direction
        @return The translation matrix
     */
    glm::mat4 GetTranslateMatrix(float translate_x, float translate_y, float translate_z);

    /**
        Get a scale matrix
        @param scale_x The amount of scaling in x direction
        @param scale_y The amount of scaling in y direction
        @param scale_z The amount of scaling in z direction
        @return The scale matrix
     */
    glm::mat4 GetScaleMatrix(float scale_x, float scale_y, float scale_z);

    /**
        Get a rotation matrix
        @param rotate_deg A rotation given in radians
        @param rotate_axis_x The x coordinate of the axis to rotate around
        @param rotate_axis_y The y coordinate of the axis to rotate around
        @param rotate_axis_z The z coordinate of the axis to rotate around
        @return The rotation matrix
     */
    glm::mat4 GetRotateMatrix(float rotate_deg, float rotate_axis_x, float rotate_axis_y, float rotate_axis_z);

}


#endif
