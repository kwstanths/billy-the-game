#include "Debugger.hpp"

#include "game_engine/math/Matrices.hpp"

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

        int ret = debug_object.Init(0, 0, 0, "assets/debug.obj");
        if (ret) return ret;

        is_inited_ = true;
        return 0;
    }

    int Debugger::Destroy() {

        debug_object.Destroy();
        renderer_= nullptr;

        is_inited_ = false;
        return 0;
    }

    bool Debugger::IsInited() {
        return is_inited_;
    }

    void Debugger::DrawPoint(float x, float y, float z, float size) {

        if (!is_inited_) return;

        debug_object.SetPosition(x, y, z);
        debug_object.Scale(size, size, size);
        debug_object.SetModelMatrix();
        renderer_->DrawSimple(&debug_object);
    }

}