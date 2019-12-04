#ifndef __Frustum_hpp__
#define __Frustum_hpp__

void multMat(float *res, float *a, float *b);

#include "game_engine/math/Vec3.hpp"
#include "game_engine/math/Vector.hpp"
#include "game_engine/math/Plane.hpp"

using namespace game_engine::math;

class AABox {
public:

    Vec3 corner;
    float x, y, z;

    AABox::AABox(Vec3 &corner, float x, float y, float z);
    AABox::AABox(void);
    AABox::~AABox();

    void AABox::setBox(Vec3 &corner, float x, float y, float z);

    // for use in frustum computations
    Vec3 AABox::getVertexP(Vec3 &normal);
    Vec3 AABox::getVertexN(Vec3 &normal);
};

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
    int FrustumG::boxInFrustum(AABox &b);
};


#endif