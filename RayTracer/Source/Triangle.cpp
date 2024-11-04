#include "Triangle.h"

bool Triangle::Hit(const ray_t& ray, rayCastHit_t& rayCastHit, float minDistance, float maxDistance)
{
	glm::vec3 edge1 = m_v2 - m_v1;
	glm::vec3 edge2 = m_v3 - m_v1;

	glm::vec3 pvec = glm::cross(ray.direction, edge2);
	float determinant = glm::dot(pvec, edge1);

	if (determinant < 0 || Math::approximately(determinant, 0))
	{
		return false;
	}

	float invDet = 1 / determinant;

	glm::vec3 tvec = ray.origin - m_v1;
	float u = glm::dot(tvec, pvec) * invDet;

	if (u < 0 || u > 1)
	{
		return false;
	}

	glm::vec3 qvec = glm::cross(tvec, edge1);
	float v = glm::dot(qvec, ray.direction) * invDet;

	if (v < 0 || u + v > 1)
	{
		return false;
	}

	float t = glm::dot(edge2, qvec) * invDet;
	if (t >= minDistance && t <= maxDistance)
	{
		rayCastHit.distance = t;
		rayCastHit.point = ray.At(t);
		rayCastHit.normal = glm::normalize(glm::cross(edge1, edge2));
		rayCastHit.material = GetMaterial();

		return true;
	}

    return false;
}
