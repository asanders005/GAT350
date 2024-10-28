#include "Material.h"
#include "Random.h"

bool Lambertian::Scatter(const ray_t& ray, const rayCastHit_t& rayCastHit, color3_t& attenuation, ray_t& scatter)
{
	scatter.origin = rayCastHit.point;
	scatter.direction = rayCastHit.normal + randomOnUnitSphere();

	attenuation = m_albedo;

    return true;
}
