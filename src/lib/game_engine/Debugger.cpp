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
        /* TODO */
    }

}