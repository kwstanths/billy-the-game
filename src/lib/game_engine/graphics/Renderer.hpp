#ifndef __Renderer_hpp__
#define __Renderer_hpp__

#include "game_engine/math/Types.hpp"
#include "game_engine/Controls.hpp"
#include "game_engine/graphics/opengl/OpenGLContext.hpp"
#include "game_engine/graphics/opengl/OpenGLRenderer.hpp"
#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/opengl/OpenGLCamera.hpp"
#include "game_engine/utility/CircularBuffer.hpp"
#include "game_engine/utility/QuadTree.hpp"

#include "GraphicsTypes.hpp"
#include "GraphicsObject.hpp"
#include "Mesh.hpp"
#include "AssetManager.hpp"

namespace game_engine {
    
    class GameEngine;

namespace graphics {

#define GAME_ENGINE_RENDERER_MAX_OBJECTS 100

    class Renderer {
        friend game_engine::GameEngine;
    public:
        Renderer();

        int Init(opengl::OpenGLContextConfig_t config);

        int Destroy();

        bool IsInited();

        void SetView();

        void StartFrame();

        void EndFrame();

        void SetWindowSize(size_t width, size_t height);

        KeyControls_t GetControlInput();

        /**
            Draws an object with full lightning. If you call this function directly, and not via the call 
            upon the rendering_object, make sure to call SetModelMatrix() on the rendering object. Also, all
            Draw() calls will happen at the end of the frame. This means that calling this Draw() on an object 
            multiple times within a single frame, will result it drawing the same object multiple on the last
            object position
            @param rendering_object The object to draw
            @return 0 = OK, -1 = rendering_object is not properly initialised
        */
        int Draw(GraphicsObject * rendering_object);

        int DrawSimple(GraphicsObject * rendering_object);

        int DrawLineXY(math::Point2D start, math::Point2D stop, float z_height, float size, glm::vec3 color);

        int DrawRectangleXY(math::Rectangle2D rect, float z_height, float size, glm::vec3 color);

        int AddPointLight(glm::vec3 position, LightProperties_t light_properties, Attenuation_t attenuation);

        int AddDirectionalLight(glm::vec3 direction, LightProperties_t light_properties);

        /**
            @param angles in degrees
        */
        int AddSpotLight(glm::vec3 position, glm::vec3 direction, Real_t inner_angle, Real_t outer_angle,
            LightProperties_t light_properties, Attenuation_t attenuation);

        int Draw2DText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        bool is_inited_;
        utility::CircularBuffer<PointLight_t> point_lights_to_draw_;
        utility::CircularBuffer<GraphicsObject *> objects_to_draw_;
        

        /* Variables needed for opengl drawiing */
        opengl::OpenGLContext * context_ = nullptr;
        opengl::OpenGLCamera * camera_ = nullptr;
        opengl::OpenGLRenderer * renderer_ = nullptr;

        /**
            Set a camera
            @param camera The camera to set
            @return 0=OK, -1 = Not initialised
        */
        int SetCamera(opengl::OpenGLCamera * camera);

        /**
            Flushes the point lights into drawing, flushes the object draw calls into drawing
        */
        void FlushDrawCalls();
    };

}
}

#endif