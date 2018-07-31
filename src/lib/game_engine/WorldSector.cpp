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

        /* TODO Find the visible items based on the z of the camera */
        size_t nof = GetObjectsWindow(rect.A_.x_/2.0 + rect.C_.x_/2.0, rect.A_.y_ / 2.0 + rect.C_.y_ / 2.0, 3, visible_world_);

        /* Step all the objects one frame */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Step(delta_time);
        }

        physics_engine_->Step();

        /*
            Set camera's view before drawing, because Step() might have tempered with the camera position
        */
        renderer->SetView();

        /* Draw visible world */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Draw(renderer);
        }

        std::vector<graphics::PointLight_t *> lights_(16);
        size_t lights = world_point_lights_.QueryRange(math::Rectangle2D(rect.A_.x_ / 2.0 + rect.C_.x_ / 2.0, rect.A_.y_ / 2.0 + rect.C_.y_ / 2.0, 50, 50), lights_);
        for (size_t i = 0; i < lights; i++)
            renderer->AddPointLight(*(lights_[i]));

        renderer->FlushDrawCalls();

        /* Update world */
        DeleteRemovedObjects();

    }

    int WorldSector::AddLight(graphics::PointLight_t * light, math::Point2D point) {

        return !world_point_lights_.Insert(point, light);
    }

    int WorldSector::Insert(WorldObject * object, Real_t x, Real_t y, Real_t z) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        size_t index_row = GetRow(y);
        size_t index_col = GetColumn(x);
        world_[index_row][index_col].push_back(object);
        /* This is probably obsolete */
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

    void WorldSector::Remove(WorldObject * object) {
        size_t index_row = GetRow(object->GetY());
        size_t index_col = GetColumn(object->GetX());

        std::deque<WorldObject *> & objects = world_[index_row][index_col];
        for (std::deque<WorldObject *>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
            if (*itr == object) {
                objects.erase(itr);
                break;
            }
        }

        if (object->removable_) {
            int ret = delete_objects_buffer_.Push(object);
            if (ret) dt::Console(dt::CRITICAL, "WorldSector::Remove(): MEMORY LEAK, delete buffer is full");

        } else dt::Console(dt::WARNING, "WorldSector::Remove(): MEMORY LEAK, removing object that was not made removable");
    }

    size_t WorldSector::GetObjectsWindow(Real_t center_x, Real_t center_y, Real_t margin,
        std::vector<WorldObject*> & objects) 
    {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::GetObjectsWindow() is not initialised");
            return -1;
        }

        size_t index = 0;
        size_t center_row = GetRow(center_y);
        size_t center_col = GetColumn(center_x);
        size_t border = static_cast<size_t>(std::ceil(margin));
        
        size_t rows_start = (center_row - border >= 0) ? (center_row - border) : 0;
        size_t rows_end = (center_row + border < world_.size()) ? (center_row + border) : (world_.size() - 1);
        size_t col_start = (center_col - border >= 0) ? (center_col - border) : 0;
        size_t col_end = (center_col + border < world_[0].size()) ? (center_col + border) : (world_[0].size() -1);

        for (size_t i = rows_start; i <= rows_end; i++) {
            for (size_t j = col_start - border; j <= col_end; j++) {
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

    size_t WorldSector::GetRow(Real_t vertical_coordinate) {
        float index = 0.0f + (world_.size() - 1 - 0.0f) * (vertical_coordinate - y_margin_start_) / (y_margin_end_ - y_margin_start_);
        return static_cast<size_t>(index);
    }

    size_t WorldSector::GetColumn(Real_t horizontal_coordinate) {
        float index = 0.0f + (world_[0].size() - 1 - 0.0f) * (horizontal_coordinate - x_margin_start_) / (x_margin_end_ - x_margin_start_);
        return static_cast<size_t>(index);
    }

    void WorldSector::DeleteRemovedObjects() {

        for (size_t i = 0; i < delete_objects_buffer_.Items(); i++) {
            WorldObject * object;
            delete_objects_buffer_.Get(object);

            ms::MemoryManager & memory_manager = ms::MemoryManager::GetInstance();
            memory_manager.GetRemovableObjectsAllocator()->Deallocate(object);
        }

    }

}

