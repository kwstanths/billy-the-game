#ifndef __Plane_hpp__
#define __Plane_hpp__

#include <glm/glm.hpp>

#include "Real.hpp"
#include "Types.hpp"

namespace game_engine { namespace math {

    class Plane {
    public:
        Plane() {};
        Plane(Vector3D p1, Vector3D p2, Vector3D p3);

        void SetPoints(Vector3D &v1, Vector3D &v2, Vector3D &v3);

        void SetNormalAndPoint(Vector3D &normal, Vector3D &point);
        
        void SetCoefficients(Real_t a, Real_t b, Real_t c, Real_t d);
        
        Real_t Distance(Vector3D &p);

        Vector3D normal_, point_;
        Real_t d_;

    };

}
}


#endif