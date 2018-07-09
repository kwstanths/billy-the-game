#include "Mesh.hpp"


namespace game_engine {
namespace graphics {

    Mesh::Mesh() {

    }

    int Mesh::Init() {

        is_inited_ = true;
        return true;
    }

    int Mesh::Destroy() {

        is_inited_ = false;
        return 0;
    }

    bool Mesh::IsInited() {
        return is_inited_;
    }


}
}