#include "Debugger.hpp"

#include "game_engine/math/HelpFunctions.hpp"

#include "ErrorCodes.hpp"

namespace grph = game_engine::graphics;
namespace gl = game_engine::graphics::opengl;

namespace game_engine {

    Debugger::Debugger() {
        is_inited_ = false;
    }

    int Debugger::Init(grph::Renderer * renderer) {

        renderer_ = renderer;

        if (!renderer->IsInited()) return Error::ERROR_GEN_NOT_INIT;
        
        int ret;
        
        ret = debug_object_point_.Init(0, 0, 0, "assets/debug.obj");
        if (ret) return ret;

        is_inited_ = true;
        return 0;
    }

    int Debugger::Destroy() {

        debug_object_point_.Destroy();
        renderer_= nullptr;

        is_inited_ = false;
        return 0;
    }

    bool Debugger::IsInited() {
        return is_inited_;
    }

    void Debugger::DrawPoint(Real_t x, Real_t y, Real_t z, Real_t size) {

        if (!is_inited_) return;

        debug_object_point_.SetPosition(x, y, z);
        debug_object_point_.Scale(size, size, size);
        renderer_->DrawSimple(&debug_object_point_);
    }

    void Debugger::DrawLineXY(math::Point2D start, math::Point2D stop, Real_t z, Real_t size, glm::vec3 color) {
        
        if (!is_inited_) return;

        renderer_->DrawLineXY(start, stop, z, size, color);
    }

    void Debugger::DrawRectangleXY(math::Rectangle2D rect, Real_t z, Real_t size, glm::vec3 color) {
        if (!is_inited_) return;

        renderer_->DrawLineXY(rect.A_, rect.B_, z, size, color);
        renderer_->DrawLineXY(rect.B_, rect.C_, z, size, color);
        renderer_->DrawLineXY(rect.C_, rect.D_, z, size, color);
        renderer_->DrawLineXY(rect.D_, rect.A_, z, size, color);
    }

}