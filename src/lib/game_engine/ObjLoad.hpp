#ifndef __ObjLoad_hpp__
#define __ObjLoad_hpp__

#include <vector>

#include <glm/glm.hpp>

namespace game_engine {

    int LoadObj(const char * path,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec2> & out_uvs,
        std::vector<glm::vec3> & out_normals);

}

#endif