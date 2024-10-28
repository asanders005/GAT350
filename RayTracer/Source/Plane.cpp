#include "Plane.h"

bool Plane::Hit(const ray_t& ray, rayCastHit_t& rayCastHit, float minDistance, float maxDistance)
{
	float denominator = glm::dot(ray.direction, m_normal);

	if (Math::approximately(denominator, 0)) return false;

	float t = glm::dot((m_center - ray.origin), m_normal) / denominator;
	if (t < 0) return false;

	if (t <= minDistance || t >= maxDistance) return false;

	rayCastHit.distance = t;
	rayCastHit.normal = glm::normalize(m_normal);
	rayCastHit.point = ray.At(t);
	rayCastHit.material = GetMaterial();

    return true;
}
