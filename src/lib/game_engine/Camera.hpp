#ifndef __Camera__hpp__
#define __Camera_hpp__

#include <glm/glm.hpp>

namespace game_engine {

    class Camera {
    public:
        Camera();

        int Init(float pos_x, float pos_y, float pos_z, 
            float dir_x, float dir_y, float dir_z,
            float up_x, float up_y, float up_z);

        int Destroy();

        glm::mat4 GetView();

    private:
        glm::vec3 position_;
        glm::vec3 direction_;
        glm::vec3 up_;
    };
}

#endif