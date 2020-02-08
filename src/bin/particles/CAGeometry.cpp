#include "CAGeometry.hpp"

//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect){
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(point, normal);
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos){
	dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point){
	float dist;
	dist = glm::dot(point, normal) + dconst;
	if (dist > 1.e-7)
		return false;
	else
		return true;
};

float Plane::distPoint2Plane(const glm::vec3& point){
	float dist;
	return dist = glm::dot(point, normal) + dconst;
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){
	glm::vec3 closestP;
	float r = (-dconst - glm::dot(point, normal));
	return closestP = point + r*normal;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	if (distPoint2Plane(point1)*distPoint2Plane(point2) > 0)	return false;
	float r = (-dconst - glm::dot(point1, normal)) / glm::dot((point2 - point1), normal);
	pTall = (1 - r)*point1 + r*point2;
	return true;
};

Triangle::Triangle(const glm::vec3 & point0, const glm::vec3 & point1, const glm::vec3 & point2) : Plane(point0, point1, point2) {
    vertex1 = point0;
    vertex2 = point1;
    vertex3 = point2;
}

Triangle::~Triangle() {

}

bool Triangle::isInside(const glm::vec3 & point) {
    return !(A(point, vertex2, vertex3) + A(vertex1, point, vertex3) + A(vertex1, vertex2, point) - A(vertex1, vertex2, vertex3) >= 1.e-1);
}

float Triangle::A(const glm::vec3 & vi, const glm::vec3 & vj, const glm::vec3 & vk) const {
    return 0.5f * glm::length(glm::cross(vj - vi, vk - vi));
}

Sphere::Sphere(const glm::vec3 & point, const float & radious)
{
    center = point;
    radi = radious;
}

Sphere::~Sphere()
{
}

void Sphere::setPosition(const glm::vec3 & newPos)
{
    center = newPos;
}

bool Sphere::isInside(const glm::vec3 & point)
{
    glm::vec3 diff = center - point;
    return diff.x * diff.x + diff.y * diff.y + diff.z * diff.z < radi * radi;
}

bool Sphere::intersecSegment(const glm::vec3 & pointa, const glm::vec3 & pointn, glm::vec3 & pTall)
{
    glm::vec3 v = pointn - pointa;

    float a = glm::dot(v, v);
    float b = 2 * glm::dot(v, pointa - center);
    float g = glm::dot(center, center) + glm::dot(pointa, pointa) - 2 * glm::dot(pointa, center) - radi * radi;

    float delta = b * b - 4 * a * g;
    if (delta < 0) return false;

    float l1 = (-b + sqrt(delta)) / (2 * a);
    float l2 = (-b - sqrt(delta)) / (2 * a);

    if (l1 <= 1 && l1 >= 0) {
        pTall = pointa + l1 * v;
        return true;
    } else if (l2 <= 1 && l2 >= 0) {
        pTall = pointa + l2 * v;
        return true;
    }

    return false;
}
