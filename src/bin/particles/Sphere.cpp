#include "Sphere.hpp"

#include "game_engine/ConsoleParser.hpp"

namespace ge = game_engine;

SphereObject::SphereObject() {
    is_inited_ = false;
}

int SphereObject::Init(game_engine::Real_t x, game_engine::Real_t y, game_engine::Real_t z, game_engine::Real_t radius, game_engine::WorldSector * world) {
    int ret = WorldObject::Init("assets/Sphere.obj", x, y, z);
    world->AddObject(this, x, y, z);

    Scale(radius, radius, radius);

    is_inited_ = true;
    return ret == 0;
}

void SphereObject::Step(double delta_time) {
    ge::ConsoleCommand c = ge::ConsoleParser::GetInstance().GetLastCommand();
    if (c.type_ == COMMAND_CA_INTEGRATION) {
        if (static_cast<int>(c.arg_1_) < 3) {
            method_ = Particle::IntegrationMethod(static_cast<int>(c.arg_1_));
        }
    }
    else if (c.type_ == COMMAND_CA_BOUNCING) {
        bouncing_ = ge::math::clamp(c.arg_1_, 0.0f, 1.0f);
    }
    else if (c.type_ == COMMAND_CA_FRICTION) {
        friction_ = ge::math::clamp(c.arg_1_, 0.0f, 1.0f);
    }
    else if (c.type_ == COMMAND_CA_ELASTICITY) {
        spring_elasticity_k_ = c.arg_1_;
    }
    else if (c.type_ == COMMAND_CA_DAMPING) {
        spring_damping_k_ = c.arg_1_;
    }
    else if (c.type_ == COMMAND_CA_DRAW_SPRING_TRIANGLES) {
        draw_spring_triangles_ = static_cast<bool>(c.arg_1_);
    }


}

void SphereObject::Draw(game_engine::graphics::Renderer * renderer) {
    std::string integration;
    if (method_ == Particle::EulerOrig) integration = "Integration: Euler original";
    else if (method_ == Particle::EulerSemi) integration = "Integration: Euler semi";
    else if (method_ == Particle::Verlet) integration = "Integration: Verlet";

    renderer->Draw2DText(integration, 0, 900, 0.4, glm::vec3(1, 0, 0));
    renderer->Draw2DText("Bouncing: " + std::to_string(bouncing_), 0, 875, 0.4, glm::vec3(1, 0, 0));
    renderer->Draw2DText("Friction: " + std::to_string(friction_), 0, 850, 0.4, glm::vec3(1, 0, 0));
    renderer->Draw2DText("Elasticity: " + std::to_string(spring_elasticity_k_), 0, 825, 0.4, glm::vec3(1, 0, 0));
    renderer->Draw2DText("Damping: " + std::to_string(spring_damping_k_), 0, 800, 0.4, glm::vec3(1, 0, 0));

    if (draw_spring_triangles_) {
        for (size_t i = 0; i < cloth_triangles_.size(); i += 3) {
            renderer->DrawTriangle(cloth_triangles_[i]->GetPositionCurrent(),
                cloth_triangles_[i + 1]->GetPositionCurrent(),
                cloth_triangles_[i + 2]->GetPositionCurrent()
                , glm::vec3(0, 1, 0));
        }
    }

    renderer->Draw(this);
}
