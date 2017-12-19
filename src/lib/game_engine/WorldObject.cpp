#include "WorldObject.hpp"

#include "OpenGLMath.hpp"

#include "ErrorCodes.hpp"

namespace game_engine {

    WorldObject::WorldObject() {

        SetPosition(0.0f, 0.0f, 0.0f);

        func_ = nullptr;

        is_inited_ = false;
    }

    int WorldObject::Init(OpenGLObject * object, OpenGLTexture * texture, OpenGLRenderer * renderer) {
        if (!object->IsInited()) {
            PrintError(Error::ERROR_OBJECT_NOT_INIT);
            return Error::ERROR_OBJECT_NOT_INIT;
        }
        if (!texture->IsInited()) {
            PrintError(Error::ERROR_TEXTURE_NOT_INIT);
            return Error::ERROR_TEXTURE_NOT_INIT;
        }

        object_ = object;
        texture_ = texture;
        renderer_ = renderer;
        
        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        

        is_inited_ = false;
        return 0;
    }

    bool WorldObject::IsInited() {
        return is_inited_;
    }

    void WorldObject::Draw() {
        if (!is_inited_) return;

        renderer_->Draw(object_, texture_, model_);
    }

    void WorldObject::Step(double delta_time) {
        if (func_ != nullptr) func_(delta_time);
    }

    void WorldObject::SetPosition(float pos_x, float pos_y, float pos_z) {
        pos_x_ = pos_x;
        pos_y_ = pos_y;
        pos_z_ = pos_z;

        model_ = GetTranslateMatrix(glm::vec3(pos_x_, pos_y_, pos_z_));

    }

    void WorldObject::SetStepFunction(std::function<void(double)> func) {
        func_ = func;
    }


}
