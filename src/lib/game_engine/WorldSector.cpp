#include "WorldSector.hpp"

#include "game_engine/memory/MemoryManager.hpp"
#include "game_engine/math/HelpFunctions.hpp"
#include "game_engine/math/Types.hpp"

#include "debug_tools/Console.hpp"
#include "debug_tools/CodeReminder.hpp"

#include "ErrorCodes.hpp"

namespace dt = debug_tools;
namespace ph = game_engine::physics;
namespace ms = game_engine::memory;
namespace math = game_engine::math;
namespace gr = game_engine::graphics;

namespace game_engine {

    WorldSector::WorldSector() {
        physics_engine_ = new ph::PhysicsEngine();
        is_inited_ = false;
    }

    int WorldSector::Init(size_t width, size_t height, 
        Real_t x_margin_start, Real_t x_margin_end,
        Real_t y_margin_start, Real_t y_margin_end,
        size_t elements) 
    {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* Initialize the world structure */
        world_ = std::vector<std::vector<std::deque<WorldObject *> > >(height, std::vector<std::deque<WorldObject *> >(width));
        
        ms::MemoryManager& memory_manager = ms::MemoryManager::GetInstance();
        world_point_lights_.Init(math::Rectangle2D(
            math::Point2D(x_margin_start, y_margin_start),
            math::Point2D(x_margin_end, y_margin_start),
            math::Point2D(x_margin_end, y_margin_end),
            math::Point2D(x_margin_start, y_margin_end)), memory_manager.GetWorldLightsAllocator());

        visible_world_ = std::vector<WorldObject *>(200);

        /* Initialize the physics engine used */
        physics_engine_->Init(math::Rectangle2D(0, 0, 250, 250), elements);

        /* Initialize the circular buffer for deleting objects */
        delete_objects_buffer_.Init(128);

        /* Set the margins */
        x_margin_start_ = x_margin_start;
        x_margin_end_ = x_margin_end;
        y_margin_start_ = y_margin_start;
        y_margin_end_ = y_margin_end;

        is_inited_ = true;
        return 0;
    }

    int WorldSector::Destroy() {
        
        CodeReminder("Iterate through the world objects, and call their Destroy()");
        
        world_.clear();
         
        is_inited_ = false;
        return 0;
    }

    bool WorldSector::IsInited() {
        return is_inited_;
    }

    void WorldSector::Step(math::Rectangle2D rect, double delta_time, gr::Renderer * renderer) {

        /* Draw a rectangle for the edge of this world */
        renderer->DrawRectangleXY(math::Rectangle2D(
            math::Point2D(x_margin_start_, y_margin_start_),
            math::Point2D(x_margin_end_, y_margin_start_),
            math::Point2D(x_margin_end_, y_margin_end_),
            math::Point2D(x_margin_start_, y_margin_end_)), 0.02f, 0.01f, { 1,1,1 }
        );

        /* Draw a rectangle for the rendering window */
        renderer->DrawRectangleXY(rect, 0.02f, 0.01f, { 1,1,1 });

        size_t nof = GetObjectsWindow(rect, visible_world_);

        /* Step all the objects one frame */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Step(delta_time);
        }

        /* Currently only calls the collision handlers */
        physics_engine_->Step();

        /* Set camera's view before drawing, because Step() might have tempered with the camera position */
        renderer->SetView();

        /* Draw visible world */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Draw(renderer);
        }

        /* Draw lights */
        std::vector<graphics::PointLight_t *> lights_(16);
        size_t lights = world_point_lights_.QueryRange(math::Rectangle2D(rect.A_.x_ / 2.0f + rect.C_.x_ / 2.0f, rect.A_.y_ / 2.0f + rect.C_.y_ / 2.0f, 50, 50), lights_);
        for (size_t i = 0; i < lights; i++)
            renderer->AddPointLight(*(lights_[i]));

        renderer->FlushDrawCalls();

        /* Update world */
        FlushObjectDelete();

    }

    int WorldSector::AddObject(WorldObject * object, Real_t x, Real_t y, Real_t z) {

        InsertObjectToWorldStructure(object, x, y, z);

        /* Set the position in the physics layer */
        physics_engine_->Insert(object, math::Point2D(x, y));
        object->PhysicsObject::SetPosition(x, y, z);

        /* Set the position in the graphics layer */
        object->GraphicsObject::SetPosition(x, y, z);

        return 0;
    }

    int WorldSector::RemoveObject(WorldObject * object) {

        RemoveObjectFromWorldStructure(object);

        physics_engine_->Remove(object);

        return 0;
    }

    int WorldSector::AddLight(graphics::PointLight_t * light, math::Point2D& point) {
        return !world_point_lights_.Insert(point, light);
    }

    int WorldSector::RemoveLight(graphics::PointLight_t * light, math::Point2D& point) {
        return !world_point_lights_.Remove(point, light);
    }

    size_t WorldSector::GetObjectsWindow(math::Rectangle2D rect, std::vector<WorldObject*> & objects)  {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::GetObjectsWindow() is not initialised");
            return -1;
        }

        /* Find starting rows and columns basd on the rectangle */
        int row_start = GetRow(rect.C_.y_);
        int row_end = GetRow(rect.A_.y_);
        int col_start = GetColumn(rect.D_.x_);
        int col_end = GetColumn(rect.B_.x_);

        /* Check margins */
        if (row_start < 0) row_start = 0;
        if (row_end >= world_.size()) row_end = world_.size() - 1;
        if (col_start < 0) col_start = 0;
        if (col_end >= world_[0].size()) col_end = world_[0].size() - 1;

        size_t index = 0;
        for (size_t i = row_start; i <= row_end; i++) {
            for (size_t j = col_start; j <= col_end; j++) {
                for (std::deque<WorldObject*>::iterator itr = world_[i][j].begin(); itr != world_[i][j].end(); ++itr) {
                    if (index > objects.size() - 1) {
                        dt::Console(dt::WARNING, "WorldSector::GetObjetcsWindow() Objects overflow");
                        return index;
                    }
                    objects[index++] = *itr;
                }

            }
        }
        return index;
    }

    WorldObject * WorldSector::FindInteractNeighbour(math::Rectangle2D search_area, math::Point2D p, Real_t size) {
        std::vector<ph::PhysicsObject *> area_objects(10, nullptr);
        size_t nof = physics_engine_->GetObjectsArea(search_area, area_objects);

        ph::PhysicsObject * neighbour = nullptr;
        /* Find the first interactable object in the area */
        for (size_t i = 0; i < nof; i++) {
            WorldObject * temp = dynamic_cast<WorldObject *>(area_objects[i]);
            if (temp->interactable_) return temp;
        }

        return nullptr;
    }

    ph::PhysicsEngine * WorldSector::GetPhysicsEngine() {

        return physics_engine_;
    }

    int WorldSector::GetRow(Real_t vertical_coordinate) {
        /* 
            If y_margin_end is mapped to the first row, and y_margin_start is mapped to the last row, then 
            vertical_coordinate is mapped to row index
        */
        Real_t index = (world_.size() - Real_t(1)) * (y_margin_end_ - vertical_coordinate) / (y_margin_end_ - y_margin_start_);
        return static_cast<int>(index);
    }

    int WorldSector::GetColumn(Real_t horizontal_coordinate) {
        /*
            If x_margin_end is mapped to the last column, and x_margin_start is mapped to the first column, then
            horizontal_coordinate is mapped to column index
        */
        Real_t index = (world_[0].size() - Real_t(1)) * (horizontal_coordinate - x_margin_start_) / (x_margin_end_ - x_margin_start_);
        return static_cast<int>(index);
    }


    int WorldSector::InsertObjectToWorldStructure(WorldObject * object, Real_t x, Real_t y, Real_t z) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        size_t index_row = GetRow(y);
        size_t index_col = GetColumn(x);
        world_[index_row][index_col].push_back(object);
        object->world_sector_ = this;

        return 0;
    }


    void WorldSector::UpdateObjectPosition(WorldObject * object, Real_t old_pos_x, Real_t old_pos_y, Real_t new_pos_x, Real_t new_pos_y) {

        /* TODO Do some checkig on th object */

        /* Find the old and new position */
        size_t old_pos_index_row = GetRow(old_pos_y);
        size_t old_pos_index_col = GetColumn(old_pos_x);
        size_t new_pos_index_row = GetRow(new_pos_y);
        size_t new_pos_index_col = GetColumn(new_pos_x);
        /* If no moving is required then leave */

        if ((old_pos_index_row == new_pos_index_row) && (old_pos_index_col == new_pos_index_col)) return;

        /* Remove from the old position */
        std::deque<WorldObject *> & objects = world_[old_pos_index_row][old_pos_index_col];
        for (std::deque<WorldObject *>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
            if (*itr == object) {
                objects.erase(itr);
                break;
            }
        }

        /* Add to new position */
        world_[new_pos_index_row][new_pos_index_col].push_back(object);
    }

    void WorldSector::RemoveObjectFromWorldStructure(WorldObject * object) {
        size_t index_row = GetRow(object->GetY());
        size_t index_col = GetColumn(object->GetX());

        std::deque<WorldObject *> & objects = world_[index_row][index_col];
        for (std::deque<WorldObject *>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
            if (*itr == object) {
                objects.erase(itr);
                break;
            }
        }

    }

    void WorldSector::DeleteObj(WorldObject * object) {

        if (object->removable_) {
            int ret = delete_objects_buffer_.Push(object);
            if (ret) dt::Console(dt::CRITICAL, "WorldSector::RemoveObjectFromWorldStructure(): MEMORY LEAK, delete buffer is full");

        } else dt::Console(dt::WARNING, "WorldSector::RemoveObjectFromWorldStructure(): MEMORY LEAK, removing object that was not made removable");

    }

    void WorldSector::FlushObjectDelete() {

        for (size_t i = 0; i < delete_objects_buffer_.Items(); i++) {
            WorldObject * object;
            delete_objects_buffer_.Get(object);

            ms::MemoryManager & memory_manager = ms::MemoryManager::GetInstance();
            memory_manager.GetRemovableObjectsAllocator()->Deallocate(object);
        }

    }

}

