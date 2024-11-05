#include "Tracer.h"
#include "Scene.h"
#include "Ray.h"
#include "MathUtils.h"

color3_t Tracer::Trace(Scene& scene, const ray_t& ray, float minDistance, float maxDistance, int depth)
{
	if (depth == 0) return color3_t{ 0 };

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
			return attenuation * Trace(scene, scatter, minDistance, maxDistance, depth - 1);
		}
		else
		{
			return rayCastHit.material.lock()->GetEmissive();
		}
		//return rayCastHit.material.lock()->GetColor();
	}

	glm::vec3 direction = glm::normalize(ray.direction);

	float t = (direction.y + 1) * 0.5f;
	color3_t color = Math::Lerp(scene.m_skyBottom, scene.m_skyTop, t);

	return color;
}
