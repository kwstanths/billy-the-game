#ifndef __Transformations_hpp__
#define __Transformations_hpp__

#include "OpenGLIncludes.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


/**
  Get a model matrix
  @param translate_vec A vector for translation
  @param scale_vec A vector for scaling
  @param rotate_deg A rotation given in degrees
  @param rotate_axis_vec The axis for rotation
  @return The Model matrix
 */
glm::mat4 GetModelMatrix(glm::vec3 translate_vec, glm::vec3 scale_vec, GLfloat rotate_deg, glm::vec3 rotate_axis_vec);

/**
  Get a translation matrix
  @param translate_vec The translation vector
  @return The translation matrix
 */
glm::mat4 GetTranslateMatrix(glm::vec3 translate_vec);

/**
  Get a scale matrix
  @param scale_vec The scale vector
  @return The scale matrix
 */
glm::mat4 GetScaleMatrix(glm::vec3 scale_vec);

/**
  Get a rotation matrix
  @param rotate_deg A rotation given in degrees
  @param rotate_axis_vec The axis rotate around
  @return The rotation matrix
 */
glm::mat4 GetRotateMatrix(GLfloat rotate_deg, glm::vec3 rotate_axis_vec);



#endif
