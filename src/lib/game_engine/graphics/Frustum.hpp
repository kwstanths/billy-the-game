#ifndef __Frustum_hpp__
#define __Frustum_hpp__

void multMat(float *res, float *a, float *b);

#include "game_engine/math/Vec3.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/math/Plane.hpp"
#include "game_engine/math/AABox.hpp"

using namespace game_engine::math;


class FrustumG{
private:

    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        NEARP,
        FARP
    };

public:

    enum { OUTSIDE, INTERSECT, INSIDE };

    Plane pl[6];

    Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
    float nearD, farD, ratio, angle, tang;
    float nw, nh, fw, fh;

    FrustumG::FrustumG();
    FrustumG::~FrustumG();

    void setFrustum(float *m);
    void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD);
    void FrustumG::setCamDef(Vec3 &p, Vec3 &l, Vec3 &u);
    int FrustumG::pointInFrustum(Vec3 &p);
    int FrustumG::sphereInFrustum(Vec3 &p, float raio);
    int FrustumG::boxInFrustum(AABox<3> &b);
};


#endif