#include "Debugger.hpp"

#include "game_engine/math/Matrices.hpp"

#include "ErrorCodes.hpp"

namespace grph = game_engine::graphics;
namespace gl = game_engine::graphics::opengl;

namespace game_engine {

    Debugger::Debugger() {
        is_inited_ = false;
    }

    int Debugger::Init(AssetManager * asset_manager, grph::Renderer * renderer) {

        renderer_ = renderer;

        if (!asset_manager->IsInited()) return Error::ERROR_GEN_NOT_INIT;
        if (!renderer->IsInited()) return Error::ERROR_GEN_NOT_INIT;

        //int ret;
        //object_circle_ = asset_manager->FindObject("assets/circle.obj", &ret);
        //texture_circle_ = asset_manager->FindTexture("assets/debug.bmp", gl::OpenGLTexture::TEXTURE_STB, &ret);

        is_inited_ = true;
        return 0;
    }

    int Debugger::Destroy() {

        renderer_= nullptr;
        object_circle_ = nullptr;
        texture_circle_ = nullptr;

        is_inited_ = false;
        return 0;
    }

    bool Debugger::IsInited() {
        return is_inited_;
    }

    void Debugger::DrawPoint(float x, float y, float z, float size) {

        if (!is_inited_) return;

        renderer_->Draw(object_circle_, texture_circle_, GetTranslateMatrix(x, y, z) * GetScaleMatrix(size, size, size));
    }

}