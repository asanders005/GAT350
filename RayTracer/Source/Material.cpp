#include "Material.h"
#include "Random.h"
#include "MathUtils.h"
#include <glm/glm.hpp>

bool Lambertian::Scatter(const ray_t& ray, const rayCastHit_t& rayCastHit, color3_t& attenuation, ray_t& scatter) const
{
	scatter.origin = rayCastHit.point;
	scatter.direction = rayCastHit.normal + randomOnUnitSphere();

	attenuation = m_albedo;

    return true;
}

bool Metal::Scatter(const ray_t& ray, const rayCastHit_t& rayCastHit, color3_t& attenuation, ray_t& scatter) const
{
	glm::vec3 reflected = Math::Reflect(ray.direction, rayCastHit.normal);

	scatter = ray_t{ rayCastHit.point, reflected + (randomOnUnitSphere() * m_fuzz) };
	attenuation = m_albedo;

    return glm::dot(scatter.direction, rayCastHit.normal) > 0;
}

bool Dielectric::Scatter(const ray_t& ray, const rayCastHit_t& rayCastHit, color3_t& attenuation, ray_t& scatter) const
{
	glm::vec3 outNormal;
	float ni_over_nt;
	float cosine;

	//Check if hitting from outside
	if (glm::dot(ray.direction, rayCastHit.normal) < 0)
	{
		outNormal = rayCastHit.normal;
		ni_over_nt = 1.0f / m_refractiveIndex;
		cosine = -glm::dot(ray.direction, rayCastHit.normal) / ray.direction.length();
	}
	else
	{
		//Hitting from inside
		outNormal = -rayCastHit.normal;
		ni_over_nt = m_refractiveIndex;
		cosine = m_refractiveIndex * glm::dot(ray.direction, rayCastHit.normal) / ray.direction.length();
	}

	glm::vec3 refracted;
	float reflectProbability = 1.0f;

	if (Math::Refract(ray.direction, rayCastHit.normal, m_refractiveIndex, refracted))
	{
		reflectProbability = Math::Schlick(cosine, m_refractiveIndex);
	}

	glm::vec3 reflected = Math::Reflect(ray.direction, rayCastHit.normal);

	scatter = (randomf() < reflectProbability) ? ray_t{ rayCastHit.point, reflected } : ray_t{ rayCastHit.point, refracted };

	attenuation = m_albedo;

    return true;
}
