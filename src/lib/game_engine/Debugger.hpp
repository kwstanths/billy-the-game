#ifndef __Debugger_hpp__
#define __Debugger_hpp__


#include "game_engine/graphics/opengl/OpenGLObject.hpp"
#include "game_engine/graphics/opengl/OpenGLTexture.hpp"
#include "game_engine/graphics/Renderer.hpp"

#include "AssetManager.hpp"


namespace game_engine {

    /**
        An object to encapsulare debugging methods
    */
    class Debugger {
    public:
        /* Does nothing, call Init() */
        Debugger();

        /**
            Does the initialization
            @param asset_manager The asset manager object
            @param renderer The renderer object
            @return 0 = OK, -1 = objects passed not initialised
        */
        int Init(AssetManager * asset_manager, graphics::Renderer * renderer);

        /**
            No deallocation needed for this objects, Does nothing of great importance 
        */
        int Destroy();

        /**
            Get whether this object is initialised
            @return true = initialised, false = not initialised
        */
        bool IsInited();

        /**
            Draw a circular point on the map of color pink
            @param x The x coordinate of the position 
            @param y The y coordinate of the position
            @param z The z coordinate of the position
            @param size The size of the point
        */
        void DrawPoint(float x, float y, float z, float size);

    private:
        bool is_inited_;

        graphics::Renderer * renderer_ = nullptr;
        graphics::opengl::OpenGLObject * object_circle_ = nullptr;
        graphics::opengl::OpenGLTexture * texture_circle_ = nullptr;
    };

}





#endif