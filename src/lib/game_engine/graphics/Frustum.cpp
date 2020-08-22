#include "Frustum.hpp"

#include <cmath>
#include <iostream>

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum() {}

Frustum::~Frustum() {}

#define m(row,col)  m[col*4+row-5]

void Frustum::SetFrustum(float * m) {
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

void Frustum::SetCamInternals(float angle, float ratio, float nearD, float farD) {

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

void Frustum::SetCamDef(Vector3D &p, Vector3D &l, Vector3D &u) {

    Vector3D dir, nc, fc, X, Y, Z;

    Z = p - l;
    Z.Normalise();

    X = Vector3D::CrossProduct(u, Z);
    X.Normalise();

    Y = Vector3D::CrossProduct(Z, X);

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

int Frustum::PointInFrustum(Vector3D &p) {

    int result = INSIDE;
    for (int i = 0; i < 6; i++) {

        if (pl[i].Distance(Vector3D(p)) < 0)
            return OUTSIDE;
    }
    return(result);

}

int Frustum::SphereInFrustum(Vector3D &p, float raio) {

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

int Frustum::BoxInFrustum(AABox<3> &b) {

    int result = INSIDE;
    for (int i = 0; i < 6; i++) {

        if (pl[i].Distance(AAboxGetVertexPositive(b, pl[i].normal_)) < 0)
            return OUTSIDE;
        else if (pl[i].Distance(AAboxGetVertexNegative(b, pl->normal_)) < 0)
            result = INTERSECT;
    }
    return(result);
}

