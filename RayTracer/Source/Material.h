#pragma once
#include "Color.h"
#include "Ray.h"

class Material
{
public:
	Material() = default;
	Material(const color3_t& albedo) : m_albedo{ albedo } {}

	virtual bool Scatter(const ray_t& ray, const rayCastHit_t& rayCastHit, color3_t& attenuation, ray_t& scatter) = 0;

	color3_t& GetColor() { return m_albedo; }

protected:
	color3_t m_albedo;
};

class Lambertian : public Material
{
public:
	Lambertian(const color3_t& albedo) : Material{ albedo } {}

	bool Scatter(const ray_t& ray, const rayCastHit_t& rayCastHit, color3_t& attenuation, ray_t& scatter) override;
};