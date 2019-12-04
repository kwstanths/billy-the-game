#include "Frustum.hpp"

#include <cmath>
#include <iostream>

#define ANG2RAD 3.14159265358979323846/180.0

AABox::AABox(Vec3 &corner, float x, float y, float z) {

    setBox(corner, x, y, z);
}

AABox::AABox(void) {
    corner.x_ = 0; corner.y_ = 0; corner.z_ = 0;

    x = 1.0f;
    y = 1.0f;
    z = 1.0f;

}

AABox::~AABox() {}

void AABox::setBox(Vec3 &corner, float x, float y, float z) {
    this->corner = corner;

    if (x < 0.0) {
        x = -x;
        this->corner.x_ -= x;
    }
    if (y < 0.0) {
        y = -y;
        this->corner.y_ -= y;
    }
    if (z < 0.0) {
        z = -z;
        this->corner.z_ -= z;
    }
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 AABox::getVertexP(Vec3 &normal) {

    Vec3 res = corner;

    if (normal.x_ > 0)
        res.x_ += x;

    if (normal.y_ > 0)
        res.y_ += y;

    if (normal.z_ > 0)
        res.z_ += z;

    return(res);
}

Vec3 AABox::getVertexN(Vec3 &normal) {

    Vec3 res = corner;

    if (normal.x_ < 0)
        res.x_ += x;

    if (normal.y_ < 0)
        res.y_ += y;

    if (normal.z_ < 0)
        res.z_ += z;

    return(res);
}

FrustumG::FrustumG() {}

FrustumG::~FrustumG() {}

#define m(row,col)  m[col*4+row-5]

void FrustumG::setFrustum(float * m) {
    pl[NEARP].SetCoefficients(
        m(3, 1) + m(4, 1),
        m(3, 2) + m(4, 2),
        m(3, 3) + m(4, 3),
        m(3, 4) + m(4, 4));
    pl[FARP].SetCoefficients(
        -m(3, 1) + m(4, 1),
        -m(3, 2) + m(4, 2),
        -m(3, 3) + m(4, 3),
        -m(3, 4) + m(4, 4));
    pl[BOTTOM].SetCoefficients(
        m(2, 1) + m(4, 1),
        m(2, 2) + m(4, 2),
        m(2, 3) + m(4, 3),
        m(2, 4) + m(4, 4));
    pl[TOP].SetCoefficients(
        -m(2, 1) + m(4, 1),
        -m(2, 2) + m(4, 2),
        -m(2, 3) + m(4, 3),
        -m(2, 4) + m(4, 4));
    pl[LEFT].SetCoefficients(
        m(1, 1) + m(4, 1),
        m(1, 2) + m(4, 2),
        m(1, 3) + m(4, 3),
        m(1, 4) + m(4, 4));
    pl[RIGHT].SetCoefficients(
        -m(1, 1) + m(4, 1),
        -m(1, 2) + m(4, 2),
        -m(1, 3) + m(4, 3),
        -m(1, 4) + m(4, 4));
}

#undef M

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

    this->ratio = ratio;
    this->angle = angle;
    this->nearD = nearD;
    this->farD = farD;

    tang = (float)tan(angle* ANG2RAD * 0.5);
    nh = nearD * tang;
    nw = nh * ratio;
    fh = farD * tang;
    fw = fh * ratio;


}

void FrustumG::setCamDef(Vec3 &p, Vec3 &l, Vec3 &u) {

    Vec3 dir, nc, fc, X, Y, Z;

    Z = p - l;
    Z.Normalize();

    X = Vec3::CrossProduct(u, Z);
    X.Normalize();

    Y = Vec3::CrossProduct(Z, X);

    nc = p - Z * nearD;
    fc = p - Z * farD;

    ntl = nc + Y * nh - X * nw;
    ntr = nc + Y * nh + X * nw;
    nbl = nc - Y * nh - X * nw;
    nbr = nc - Y * nh + X * nw;

    ftl = fc + Y * fh - X * fw;
    ftr = fc + Y * fh + X * fw;
    fbl = fc - Y * fh - X * fw;
    fbr = fc - Y * fh + X * fw;

    pl[TOP].SetPoints(Vector3D(ntr), Vector3D(ntl), Vector3D(ftl));
    pl[BOTTOM].SetPoints(Vector3D(nbl), Vector3D(nbr), Vector3D(fbr));
    pl[LEFT].SetPoints(Vector3D(ntl), Vector3D(nbl), Vector3D(fbl));
    pl[RIGHT].SetPoints(Vector3D(nbr), Vector3D(ntr), Vector3D(fbr));
    pl[NEARP].SetPoints(Vector3D(ntl), Vector3D(ntr), Vector3D(nbr));
    pl[FARP].SetPoints(Vector3D(ftr), Vector3D(ftl), Vector3D(fbl));
}

int FrustumG::pointInFrustum(Vec3 &p) {

    int result = INSIDE;
    for (int i = 0; i < 6; i++) {

        if (pl[i].Distance(Vector3D(p)) < 0)
            return OUTSIDE;
    }
    return(result);

}

int FrustumG::sphereInFrustum(Vec3 &p, float raio) {

    int result = INSIDE;
    float distance;

    for (int i = 0; i < 6; i++) {
        distance = pl[i].Distance(Vector3D(p));
        if (distance < -raio)
            return OUTSIDE;
        else if (distance < raio)
            result = INTERSECT;
    }
    return(result);
}

int FrustumG::boxInFrustum(AABox &b) {

    int result = INSIDE;
    for (int i = 0; i < 6; i++) {
        Vec3 norma = Vec3(pl[i].normal_.x(), pl[i].normal_.y(), pl[i].normal_.z());

        if (pl[i].Distance(Vector3D(b.getVertexP(norma))) < 0)
            return OUTSIDE;
        else if (pl[i].Distance(Vector3D(b.getVertexN(norma))) < 0)
            result = INTERSECT;
    }
    return(result);
}

void multMat(float * res, float * a, float * b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i * 4 + j] = 0.0;
            for (int k = 0; k < 4; k++) {
                res[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
            }
        }
    }
}
