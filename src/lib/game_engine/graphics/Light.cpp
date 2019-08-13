#include "Light.hpp"
#include "Renderer.hpp"

namespace game_engine {
namespace graphics {

    void PointLight::DrawLight(graphics::Renderer * renderer) {
        renderer->AddPointLight(this);
    }

    void PointLight::StepLight(double delta_time) {

    }

    void DirectionalLight::DrawLight(graphics::Renderer * renderer) {
        renderer->AddDirectionalLight(this);
    }

    void DirectionalLight::StepLight(double delta_time) {

    }

    void SpotLight::DrawLight(graphics::Renderer * renderer) {
        renderer->AddSpotLight(this);
    }

    void SpotLight::StepLight(double delta_time) {
    
    }

}
}
