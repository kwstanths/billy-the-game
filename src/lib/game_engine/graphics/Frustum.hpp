#ifndef __Frustum_hpp__
#define __Frustum_hpp__

void multMat(float *res, float *a, float *b);

#include "game_engine/math/Vec3.hpp"

//class Vec3 {
//public:
//
//    float x, y, z;
//
//    Vec3::Vec3(float x, float y, float z);
//    Vec3::Vec3(const Vec3 &v);
//    Vec3::Vec3();
//    Vec3::~Vec3();
//
//    Vec3 Vec3::operator +(Vec3 &v);
//    Vec3 Vec3::operator -(const Vec3 &v);
//    Vec3 Vec3::operator *(Vec3 &v);
//    Vec3 Vec3::operator *(float t);
//    Vec3 Vec3::operator /(float t);
//    Vec3 Vec3::operator -(void);
//
//    float Vec3::length();
//    void Vec3::normalize();
//    float Vec3::innerProduct(Vec3  &v);
//    void copy(const Vec3 &v);
//    void set(float x, float y, float z);
//    Vec3 scalarMult(float a);
//
//    void Vec3::print();
//};

using namespace game_engine::math;

class Plane {
public:

    Vec3 normal, point;
    float d;

    Plane::Plane(Vec3 &v1, Vec3 &v2, Vec3 &v3);
    Plane::Plane(void);
    Plane::~Plane();

    void set3Points(Vec3 &v1, Vec3 &v2, Vec3 &v3);
    void setNormalAndPoint(Vec3 &normal, Vec3 &point);
    void setCoefficients(float a, float b, float c, float d);
    float distance(Vec3 &p);

    void print();
};

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