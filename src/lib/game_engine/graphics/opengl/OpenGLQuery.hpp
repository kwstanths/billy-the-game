#ifndef __OpenGLQuery_hpp__
#define __OpenGLQuery_hpp__

#include "OpenGLIncludes.hpp"

#include <glm/glm.hpp>

namespace game_engine {
namespace graphics {
namespace opengl {

    class OpenGLQuery {
    public:
        OpenGLQuery();

        int Init();

        int Destroy();

        int Begin(int query_type);

        int End();

        int GetResult();

        bool IsResultReady();
        
    private:
        bool is_inited_;

        int type;
        GLuint query_;
    };

}
}
}

#endif
