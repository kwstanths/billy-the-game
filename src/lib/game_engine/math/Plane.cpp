#include "Plane.hpp"

namespace game_engine { namespace math {
    
    Plane::Plane(Vector3D p1, Vector3D p2, Vector3D p3) {
        SetPoints(p1, p2, p3);
    }

    void Plane::SetPoints(Vector3D & v1, Vector3D & v2, Vector3D & v3) {
        Vector3D aux1, aux2;

        aux1 = v1 - v2;
        aux2 = v3 - v2;

        normal_ = Vector3D::CrossProduct(aux2, aux1);

        normal_.Normalise();
        point_ = v2;
        d_ = -(normal_.DotProduct(point_));
    }

    void Plane::SetNormalAndPoint(Vector3D & normal, Vector3D & point) {
        normal_ = normal;
        normal_.Normalise();
        d_ = -(normal_.DotProduct(point));
    }

    void Plane::SetCoefficients(Real_t a, Real_t b, Real_t c, Real_t d) {
        /* set the normal vector */
        normal_ = Vector3D({ a, b, c });
        /* compute the lenght of the vector */
        float l = normal_.Norm();
        /* normalize the vector */
        normal_ = Vector3D({ a / l, b / l, c / l });
        /* and divide d by th length as well */
        d_ = d / l;
    }

    Real_t Plane::Distance(Vector3D & p) {
        return (d_ + normal_.DotProduct(p));
    }

}
}

