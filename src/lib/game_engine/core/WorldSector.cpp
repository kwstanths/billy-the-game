#include "WorldSector.hpp"

#include "ConfigurationFile.hpp"

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
        physics_engine_ = new physics::PhysicsEngine();
        is_inited_ = false;
    }

    int WorldSector::Init(size_t width, size_t height, 
        Real_t x_margin_start, Real_t x_margin_end,
        Real_t y_margin_start, Real_t y_margin_end,
        size_t elements) 
    {
        if (is_inited_) return Error::ERROR_GEN_NOT_INIT;

        /* width is number of columns, height is number of rows */
        /* x direction traverses the columns, y direction traverses the rows */

        /* Set the margins */
        grid_rows_ = height;
        grid_columns_ = width;

        x_margin_start_ = x_margin_start;
        x_margin_end_ = x_margin_end;
        y_margin_start_ = y_margin_start;
        y_margin_end_ = y_margin_end;
        world_window_ = AABox<2>(Vector2D(x_margin_start, y_margin_start), Vector2D(x_margin_end, y_margin_end));

        /* Initialize the world grid */
        world_ = new utility::UniformGrid<std::deque<WorldObject *>, 2>({ grid_rows_, grid_columns_});
        visible_world_ = std::vector<WorldObject *>(650, nullptr);

        /* Initialize point lights data structure */
        world_point_lights_ = new utility::QuadTree<graphics::PointLight *>(math::Vector2D(x_margin_start_, y_margin_start_), std::max(y_margin_end_ - y_margin_start_, x_margin_end_ - x_margin_start_));

        /* Initialize physics engine */
        physics_engine_->Init(AABox<2>(Vector2D(x_margin_start_, y_margin_start), Vector2D(x_margin_end_, y_margin_end)), 500);
        
        /* Initialize the circular buffer for deleting objects */
        delete_objects_buffer_.Init(128);

        use_visible_world_window_ = ConfigurationFile::GetInstance().UseVisibleWindow();

        {
            /* 
                The Quad tree data structure for ray casting, requires that the world is centered around (0,0)
                Calcualte it in a way, so that this is always true
            */
            Real_t max_x = std::max(std::abs(x_margin_start_), std::abs(x_margin_end_));
            Real_t max_y = std::max(std::abs(y_margin_start_), std::abs(y_margin_end_));
            Real_t max = std::max(max_x, max_y);
            /* Initialize acceleration data structure for ray casting */
            interaction_tree_ = new utility::QuadTreeBoxes<Interactablebject *>(math::Vector2D(-max), 2 * max);
        }

        is_inited_ = true;
        return 0;
    }

    int WorldSector::Destroy() {
        
        CodeReminder("Iterate through the world objects, and call their Destroy()");
        
        delete world_;
         
        is_inited_ = false;
        return 0;
    }

    bool WorldSector::IsInited() {
        return is_inited_;
    }

    void WorldSector::Step(double delta_time, gr::Renderer * renderer, math::Vector3D camera_position, math::Vector3D camera_direction, Real_t camera_ratio, Real_t camera_angle) {

        /* Caclulate visible window, camera looks down the z axis, and the world is at z=0 on the xy pane */
        Real_t width = camera_position.z() * tan(camera_angle / 2.0f);
        /* (2 * width) whould be exactly inside the camera view, 5 times should be more than enough */
        math::AABox<2> camera_view_box = math::AABox<2>(Vector2D(camera_position.x(), camera_position.y()), { 5.0f * width * camera_ratio, 5.0f * width });

        /* Draw a rectangle for the edge of this world */
        //renderer->DrawRectangleXY(math::Rectangle2D(
        //    math::Point2D(x_margin_start_, y_margin_start_),
        //    math::Point2D(x_margin_end_, y_margin_start_),
        //    math::Point2D(x_margin_end_, y_margin_end_),
        //    math::Point2D(x_margin_start_, y_margin_end_)), 0.02f, 0.01f, { 1,1,1 }
        //);

        /* Draw a rectangle for the rendering window */
        //renderer->DrawRectangleXY(camera_view_rectangle, 0.02f, 0.01f, { 1,1,1 });

        /* Draw everything? or only the visible part based on the 2D grid logic? */
        size_t nof;
        if (use_visible_world_window_)
            nof = GetObjectsWindow(camera_view_box, visible_world_);
        else
            nof = GetObjectsWindow(world_window_, visible_world_);

        /* Step all the objects one frame */
        for (size_t i = 0; i < nof; i++) {
            WorldObject * visible_object = visible_world_[i];
            visible_object->Step(delta_time);
        }
        if (directional_light_ != nullptr) directional_light_->StepLight(delta_time);

        /* Draw visible world */
        for (size_t i = 0; i < nof; i++) {
            visible_world_[i]->Draw(renderer);
        }
        if (directional_light_ != nullptr) directional_light_->DrawLight(renderer);
        else {
            /* If world has no light, initialize an empty light */
            graphics::DirectionalLight empty_light(0);
            renderer->AddDirectionalLight(&empty_light);
        }

        /* Draw all point lights */
        width = 3.0f * camera_position.z() * tan(camera_angle / 2.0f);
        /* (2 * width) whould be exactly inside the camera view, 4* gives us a little bigger rectangle */
        math::AABox<2> camera_view_lights_box = math::AABox<2>(Vector2D(camera_position.x(), camera_position.y()), { 1.4f * width * camera_ratio, 1.4f * width });

        /* Get all point lights within the visible world */
        std::vector<graphics::PointLight *> lights_;
        if (use_visible_world_window_)
            world_point_lights_->QueryRange(camera_view_lights_box, lights_);
        else
            world_point_lights_->QueryRange(world_window_, lights_);
        
        /* Step and draw the lights */
        for (size_t i = 0; i < lights_.size(); i++) {
            lights_[i]->StepLight(delta_time);
            lights_[i]->DrawLight(renderer);
        }

        FlushObjectDelete();
    }

    int WorldSector::AddObject(WorldObject * object, Real_t x, Real_t y, Real_t z) {

        InsertObjectToWorldStructure(object, x, y, z);

        /* Set the position in the graphics layer */
        object->GraphicsObject::SetPosition(x, y, z);

        /* Set the position in the physics layer */
        object->PhysicsObject::SetPosition(x, y, z);

        return 0;
    }

    int WorldSector::RemoveObject(WorldObject * object) {
        
        _assert(0 == 1);

        /* Remove from world structure */
        RemoveObjectFromWorldStructure(object);

        physics_engine_->Remove(object);

        return 0;
    }

    int WorldSector::AddPointLight(graphics::PointLight * light, math::Vector2D& point) {
        return !world_point_lights_->Insert(point, light);
    }

    int WorldSector::RemovePointLight(graphics::PointLight * light, math::Vector2D& point) {
        world_point_lights_->Remove(point);
        return 0;
    }

    void WorldSector::SetDirectionalLight(graphics::DirectionalLight * light) {
        directional_light_ = light;
    }

    void WorldSector::RemoveDirectionalLight() {
        directional_light_ = nullptr;
    }

    int WorldSector::AddInterractableObject(Interactablebject * object, AABox<2> interaction_area) {
        return interaction_tree_->Insert(object, interaction_area);
    }

    Interactablebject * WorldSector::RayCast(math::Ray2D ray) {
        std::vector<Interactablebject *> results;
        bool ret = interaction_tree_->RayCast(ray, results);
        
        /* Return the first result */
        if (ret) return results[0];
        return nullptr;
    }

    size_t WorldSector::GetObjectsWindow(math::AABox<2> rect, std::vector<WorldObject*> & objects)  {
        if (!is_inited_) {
            dt::Console(dt::CRITICAL, "WorldSector::GetObjectsWindow() is not initialised");
            return -1;
        }

        /* Find starting rows and columns basd on the rectangle */

        int row_start = GetRow(rect.max_[1]);
        int row_end = GetRow(rect.min_[1]);
        int col_start = GetColumn(rect.min_[0]);
        int col_end = GetColumn(rect.max_[0]);
        if (row_end < row_start) std::swap(row_start, row_end);
        if (col_end < col_start) std::swap(col_start, col_end);

        /* Check margins */
        if (row_start < 0) row_start = 0;
        if (row_end >= grid_rows_) row_end = grid_rows_ - 1;
        if (col_start < 0) col_start = 0;
        if (col_end >= grid_columns_) col_end = grid_columns_ - 1;

        size_t index = 0;
        for (int i = row_start; i <= row_end; i++) {
            for (int j = col_start; j <= col_end; j++) {
                for (std::deque<WorldObject*>::iterator itr = world_->at(i, j).begin(); itr != world_->at(i, j).end(); ++itr) {
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

    physics::PhysicsEngine * WorldSector::GetPhysicsEngine() {
        return physics_engine_;
    }

    int WorldSector::GetRow(Real_t y_coordinate) {
        /* 
            If y_margin_end is mapped to the first row, and y_margin_start is mapped to the last row, then 
            vertical_coordinate is mapped to row index
        */

        int index = math::map_to_range<Real_t, int>(y_margin_start_, y_margin_end_, y_coordinate, 0, (int)grid_rows_ - 1);
        return static_cast<int>(std::round(index));
    }

    int WorldSector::GetColumn(Real_t x_coordinate) {
        /*
            If x_margin_end is mapped to the last column, and x_margin_start is mapped to the first column, then
            horizontal_coordinate is mapped to column index
        */

        Real_t index = math::map_to_range(x_margin_start_, x_margin_end_, x_coordinate, 0, (int)grid_columns_ - 1);
        return static_cast<int>(std::round(index));
    }


    int WorldSector::InsertObjectToWorldStructure(WorldObject * object, Real_t x, Real_t y, Real_t z) {
        if (!is_inited_) return Error::ERROR_GEN_NOT_INIT;

        size_t index_row = GetRow(y);
        size_t index_col = GetColumn(x);

        world_->at(index_row, index_col).push_back(object);
        object->world_sector_ = this;

        return 0;
    }

    void WorldSector::UpdateObjectInWorldStructure(WorldObject * object, Real_t old_pos_x, Real_t old_pos_y, Real_t new_pos_x, Real_t new_pos_y) {

        /* TODO Do some checkig on the object */

        /* Find the old and new position */
        size_t old_pos_index_row = GetRow(old_pos_y);
        size_t old_pos_index_col = GetColumn(old_pos_x);
        size_t new_pos_index_row = GetRow(new_pos_y);
        size_t new_pos_index_col = GetColumn(new_pos_x);
        /* If no moving is required then leave */

        if ((old_pos_index_row == new_pos_index_row) && (old_pos_index_col == new_pos_index_col)) return;

        /* Remove from the old position */
        std::deque<WorldObject *> & objects = world_->at(old_pos_index_row, old_pos_index_col);
        for (std::deque<WorldObject *>::iterator itr = objects.begin(); itr != objects.end(); ++itr) {
            if (*itr == object) {
                objects.erase(itr);
                break;
            }
        }

        /* Add to new position */
        world_->at(new_pos_index_row, new_pos_index_col).push_back(object);
    }

    void WorldSector::RemoveObjectFromWorldStructure(WorldObject * object) {
        size_t index_row = GetRow(object->GetY());
        size_t index_col = GetColumn(object->GetX());

        std::deque<WorldObject *> & objects = world_->at(index_row, index_col);
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
            if (ret) dt::Console(dt::CRITICAL, "WorldSector::DeleteObj(): MEMORY LEAK, delete buffer is full");

        } else dt::Console(dt::WARNING, "WorldSector::DeleteObj(): MEMORY LEAK, removing object that was not made removable");

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

