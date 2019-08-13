#ifndef __Light_hpp__
#define __Light_hpp__

#include <glm/glm.hpp>

#include "GraphicsTypes.hpp"

#include "game_engine/math/Real.hpp"

namespace game_engine {
namespace graphics {

    class Renderer;

    class Light {
    public:
        /**
            Creates pure white light
        */
        Light() {
            ambient_ = glm::vec3(1.0f, 1.0f, 1.0f);
            diffuse_ = glm::vec3(1.0f, 1.0f, 1.0f);
            specular_ = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        /**
            Creates custom light
            @param a All light components are equal to a
        */
        Light(Real_t a) {
            ambient_ = glm::vec3(a, a, a);
            diffuse_ = glm::vec3(a, a, a);
            specular_ = glm::vec3(a, a, a);
        }

        /**
            Creates a custom light
        */
        Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : ambient_(ambient), diffuse_(diffuse), specular_(specular) {};

        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        
        virtual void DrawLight(graphics::Renderer * renderer) = 0;

        virtual void StepLight(double delta_time) = 0;
    };

    /**
        Holds the necessary values to draw a point light
    */
    class PointLight: public Light {
    public:
        PointLight(): Light() {};

        PointLight(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Attenuation_t att) : Light(ambient, diffuse, specular), position_(pos), attenutation_(att) {};

        glm::vec3 position_ = glm::vec3(0, 0, 0);
        
        Attenuation_t attenutation_;

        void DrawLight(graphics::Renderer * renderer);
        
        void StepLight(double delta_time);
    };

    /**
        Holds the necessary values to draw a directional light
    */
    class DirectionalLight : public Light {
    public:
        DirectionalLight() : Light() {};

        DirectionalLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : Light(ambient, diffuse, specular), direction_(dir) {};

        glm::vec3 direction_ = glm::vec3(0, 0, -1);
        
        void DrawLight(graphics::Renderer * renderer);

        void StepLight(double delta_time);
    };

    class SpotLight : public Light {
    public:
        SpotLight() : Light() {};

        SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Attenuation_t att, Real_t inner_angle, Real_t outer_angle) :
            Light(ambient, diffuse, specular), position_(pos), direction_(dir), attenuation_(att), inner_angle_(inner_angle), outer_angle_(outer_angle) {};

        glm::vec3 position_ = glm::vec3(0, 0, 0);
        glm::vec3 direction_ = glm::vec3(0, 0, -1);

        Real_t inner_angle_, outer_angle_;

        Attenuation_t attenuation_;

        void DrawLight(graphics::Renderer * renderer);
      
        void StepLight(double delta_time);
    };

}
}

#endif