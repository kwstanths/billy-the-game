#include "Frustum.hpp"

#include <cmath>
#include <iostream>

#define ANG2RAD 3.14159265358979323846/180.0
//
//Vec3::Vec3(float x, float y, float z) {
//
//    this->x = x;
//    this->y = y;
//    this->z = z;
//
//}
//
//Vec3::Vec3(const Vec3 &v) {
//
//    x = v.x;
//    y = v.y;
//    z = v.z;
//
//}
//
//
//Vec3::Vec3() {
//
//    x = 0;
//    y = 0;
//    z = 0;
//
//}
//
//
//Vec3::~Vec3()
//{
//
//}
//
////////////////////////////////////////////////////////////////////////
//// Methods
////////////////////////////////////////////////////////////////////////
//
//Vec3 Vec3::operator+(Vec3 &v) {
//
//    Vec3 res;
//
//    res.x = x + v.x;
//    res.y = y + v.y;
//    res.z = z + v.z;
//
//    return(res);
//}
//
//Vec3 Vec3::operator-(const Vec3 &v) {
//
//    Vec3 res;
//
//    res.x = x - v.x;
//    res.y = y - v.y;
//    res.z = z - v.z;
//
//    return(res);
//}
//
//Vec3 Vec3::operator-(void) {
//
//    Vec3 res;
//
//    res.x = -x;
//    res.y = -y;
//    res.z = -z;
//
//    return(res);
//}
//
//// cross product
//Vec3 Vec3::operator*(Vec3 &v) {
//
//    Vec3 res;
//
//    res.x = y * v.z - z * v.y;
//    res.y = z * v.x - x * v.z;
//    res.z = x * v.y - y * v.x;
//
//    return (res);
//}
//
//Vec3 Vec3::operator*(float t) {
//
//    Vec3 res;
//
//    res.x = x * t;
//    res.y = y * t;
//    res.z = z * t;
//
//    return (res);
//}
//
//
//Vec3 Vec3::operator/(float t) {
//
//    Vec3 res;
//
//    res.x = x / t;
//    res.y = y / t;
//    res.z = z / t;
//
//    return (res);
//}
//
//
//
//float Vec3::length() {
//
//    return((float)std::sqrt(x*x + y * y + z * z));
//}
//
//void Vec3::normalize() {
//
//    float len;
//
//    len = length();
//    if (len) {
//        x /= len;;
//        y /= len;
//        z /= len;
//    }
//}
//
//
//float Vec3::innerProduct(Vec3 &v) {
//
//    return (x * v.x + y * v.y + z * v.z);
//}
//
//void Vec3::copy(const Vec3 &v) {
//
//    x = v.x;
//    y = v.y;
//    z = v.z;
//}
//
//void Vec3::set(float x, float y, float z) {
//
//    this->x = x;
//    this->y = y;
//    this->z = z;
//}
//
//Vec3 Vec3::scalarMult(float a) {
//
//    Vec3 res;
//
//    res.x = x * a;
//    res.y = y * a;
//    res.z = z * a;
//
//    return res;
//}
//
//
//// -----------------------------------------------------------------------
//// For debug pruposes: Prints a vector
//// -----------------------------------------------------------------------
//
//
//void Vec3::print() {
//    printf("Vec3(%f, %f, %f)", x, y, z);
//}

Plane::Plane(Vec3 &v1, Vec3 &v2, Vec3 &v3) {

    set3Points(v1, v2, v3);
}


Plane::Plane() {}

Plane::~Plane() {}


void Plane::set3Points(Vec3 &v1, Vec3 &v2, Vec3 &v3) {


    Vec3 aux1, aux2;

    aux1 = v1 - v2;
    aux2 = v3 - v2;

    normal = aux2.CrossProduct(aux1);

    normal.Normalize();
    point = v2;
    d = -(normal.DotProduct(point));
}

void Plane::setNormalAndPoint(Vec3 &normal, Vec3 &point) {

    this->normal = normal;
    this->normal.Normalize();
    d = -(this->normal.DotProduct(point));
}

void Plane::setCoefficients(float a, float b, float c, float d) {

    // set the normal vector
    normal.Set(a, b, c);
    //compute the lenght of the vector
    float l = normal.Norm();
    // normalize the vector
    normal.Set(a / l, b / l, c / l);
    // and divide d by th length as well
    this->d = d / l;
}

float Plane::distance(Vec3 &p) {

    return (d + normal.DotProduct(p));
}

void Plane::print() {

    //printf("Plane("); normal.print(); printf("# %f)", d);
}

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
    pl[NEARP].setCoefficients(
        m(3, 1) + m(4, 1),
        m(3, 2) + m(4, 2),
        m(3, 3) + m(4, 3),
        m(3, 4) + m(4, 4));
    pl[FARP].setCoefficients(
        -m(3, 1) + m(4, 1),
        -m(3, 2) + m(4, 2),
        -m(3, 3) + m(4, 3),
        -m(3, 4) + m(4, 4));
    pl[BOTTOM].setCoefficients(
        m(2, 1) + m(4, 1),
        m(2, 2) + m(4, 2),
        m(2, 3) + m(4, 3),
        m(2, 4) + m(4, 4));
    pl[TOP].setCoefficients(
        -m(2, 1) + m(4, 1),
        -m(2, 2) + m(4, 2),
        -m(2, 3) + m(4, 3),
        -m(2, 4) + m(4, 4));
    pl[LEFT].setCoefficients(
        m(1, 1) + m(4, 1),
        m(1, 2) + m(4, 2),
        m(1, 3) + m(4, 3),
        m(1, 4) + m(4, 4));
    pl[RIGHT].setCoefficients(
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

    pl[TOP].set3Points(ntr, ntl, ftl);
    pl[BOTTOM].set3Points(nbl, nbr, fbr);
    pl[LEFT].set3Points(ntl, nbl, fbl);
    pl[RIGHT].set3Points(nbr, ntr, fbr);
    pl[NEARP].set3Points(ntl, ntr, nbr);
    pl[FARP].set3Points(ftr, ftl, fbl);
}

int FrustumG::pointInFrustum(Vec3 &p) {

    int result = INSIDE;
    for (int i = 0; i < 6; i++) {

        if (pl[i].distance(p) < 0)
            return OUTSIDE;
    }
    return(result);

}

int FrustumG::sphereInFrustum(Vec3 &p, float raio) {

    int result = INSIDE;
    float distance;

    for (int i = 0; i < 6; i++) {
        distance = pl[i].distance(p);
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

        if (pl[i].distance(b.getVertexP(pl[i].normal)) < 0)
            return OUTSIDE;
        else if (pl[i].distance(b.getVertexN(pl[i].normal)) < 0)
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
