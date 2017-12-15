#include "WorldObject.hpp"

namespace game_engine {

    WorldObject::WorldObject() {
        is_inited_ = false;
    }

    int WorldObject::Init(OpenGLObjectConfig_t obj_config, OpenGLContext * context) {

        int ret = OpenGLObject::Init(obj_config, context);
        if (ret != 0) return ret;


        is_inited_ = true;
        return 0;
    }

    int WorldObject::Destroy() {
        
        OpenGLObject::Destroy();

        is_inited_ = false;
        return 0;
    }

    bool WorldObject::IsInited() {
        return is_inited_;
    }


}
