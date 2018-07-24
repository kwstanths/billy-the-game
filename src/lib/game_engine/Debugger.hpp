#ifndef __Debugger_hpp__
#define __Debugger_hpp__

#include "game_engine/graphics/GraphicsObject.hpp"
#include "game_engine/graphics/Renderer.hpp"
#include "game_engine/math/Types.hpp"

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
        int Init(graphics::Renderer * renderer);

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
        void DrawPoint(Real_t x, Real_t y, Real_t z, Real_t size);

        /**
        
        */
        void DrawLineXY(math::Point2D start, math::Point2D stop, Real_t z, Real_t size, glm::vec3 color);

        void DrawRectangleXY(math::Rectangle2D rect, Real_t z, Real_t size, glm::vec3 color);

    private:
        bool is_inited_;

        graphics::GraphicsObject debug_object_point_;
        graphics::GraphicsObject debug_object_line_;
        graphics::Renderer * renderer_ = nullptr;
    };

}





#endif