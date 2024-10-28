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
