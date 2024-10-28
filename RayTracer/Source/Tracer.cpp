#include "Tracer.h"
#include "Scene.h"
#include "Ray.h"
#include "MathUtils.h"

color3_t Tracer::Trace(Scene& scene, const ray_t& ray, float minDistance, float maxDistance)
{
	rayCastHit_t rayCastHit;
	float closestDistance = maxDistance;
	bool isHit = false;

	//Check for object hit
	for (auto& object : scene.m_objects)
	{
		if (object->Hit(ray, rayCastHit, minDistance, closestDistance))
		{
			isHit = true;
			closestDistance = rayCastHit.distance;
		}
	}

	if (isHit)
	{
		color3_t attenuation;
		ray_t scatter;
		if (rayCastHit.material.lock()->Scatter(ray, rayCastHit, attenuation, scatter))
		{
			return attenuation * Trace(scene, scatter, minDistance, maxDistance);
		}
		//return rayCastHit.material.lock()->GetColor();
	}

	glm::vec3 direction = glm::normalize(ray.direction);

	float t = (direction.y + 1) * 0.5f;
	color3_t color = Math::Lerp(color3_t{ 1, 1, 1 }, color3_t{ 0.5f, 0.7f, 1.0f }, t);

	return color;
}
