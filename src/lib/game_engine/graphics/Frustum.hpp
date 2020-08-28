#ifndef __Frustum_hpp__
#define __Frustum_hpp__

#include "game_engine/math/Vector.hpp"
#include "game_engine/math/Plane.hpp"
#include "game_engine/math/AABox.hpp"

using namespace game_engine::math;

/* A class used to represent a view frustum */
class Frustum {
private:

    /* Planes */
    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        NEARP,
        FARP
    };

public:

    /* Values returned */
    enum { OUTSIDE, INTERSECT, INSIDE };

    /* The six planes */
    Plane pl[6];

    Vector3D ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
    float nearD, farD, ratio, angle, tang;
    float nw, nh, fw, fh;

    Frustum();
    ~Frustum();

    void SetFrustum(float *m);
    void SetCamInternals(float angle, float ratio, float nearD, float farD);
    void SetCamDef(Vector3D &p, Vector3D &l, Vector3D &u);
    int PointInFrustum(Vector3D &p);
    int SphereInFrustum(Vector3D &p, float raio);
    int BoxInFrustum(AABox<3> &b);
};


#endif