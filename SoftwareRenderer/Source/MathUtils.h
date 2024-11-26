#pragma once
#define FLT_EPSILON 1.192092896e-07F
#include <random>
#include <glm/glm.hpp>

namespace Math
{
	template<typename T>
	inline T Clamp(const T& value, const T& min, const T& max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	inline int Random(int min, int max)
	{
		return (std::rand() % (max - min) + 1) + min;
	}

	inline bool approximately(float value1, float value2)
	{
		// check if the difference between the values is less than epsilon
		return (std::fabs(value1 - value2) < FLT_EPSILON);
	}

	template<typename T>
	inline T Lerp(const T& a, const T& b, float t)
	{
		return static_cast<T>(a + (t * (b - a)));
	}

	inline glm::vec3 Reflect(const glm::vec3& incident, const glm::vec3& normal)
	{
		return incident - (normal * glm::dot(normal, incident)) * 2.0f;
	}

	inline bool Refract(const glm::vec3& incident, const glm::vec3& normal, float refractiveIndex, glm::vec3& refracted)
	{
		glm::vec3 normalIncident = glm::normalize(incident);
		float cosine = glm::dot(normalIncident, normal);

		float discriminant = 1 - (refractiveIndex * refractiveIndex) * (1 - cosine * cosine);
		if (discriminant > 0)
		{
			refracted = refractiveIndex * (normalIncident - (normal * cosine)) - (normal * glm::sqrt(discriminant));
			return true;
		}

		return false;
	}

	inline float Schlick(float cosine, float index)
	{
		// Step 1: Calculate the base reflectance at zero incidence (angle = 0)
		// This is the reflection coefficient when the light hits the surface straight on
		float r0 = (1.0f - index) / (1.0f + index);
		r0 = r0 * r0;

		// Step 2: Use Schlick's approximation to adjust reflectance based on angle
		// Schlick�s approximation gives the probability of reflection at an angle `cosine`
		// It interpolates between `r0` and 1, with stronger reflection at glancing angles
		return r0 + (1.0f - r0) * (float)std::pow((1.0f - cosine), 5);
	}

	inline float Dot(const glm::vec3& v1, const glm::vec3& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}

	inline float Angle(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::acos(glm::dot(glm::normalize(v1), glm::normalize(v2)));
	}

	inline float Cross(const glm::vec2& v1, const glm::vec2& v2)
	{
		return glm::cross(glm::vec3{ v1.x, v1.y, 0 }, glm::vec3{ v2.x, v2.y, 0 }).z;
	}

	inline glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2)
	{
		glm::vec3 result;

		result.x = (v1.y * v2.z) - (v2.y * v1.z);
		result.y = (v1.z * v2.x) - (v2.z * v1.x);
		result.z = (v1.x * v2.y) - (v2.x * v1.y);

		return result;
	}

	inline void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y)
	{
		float omt = 1 - t;

		float a = omt * omt;
		float b = 2 * omt * t;
		float c = t * t;

		x = (int)((a * x1) + (b * x2) + (c * x3));
		y = (int)((a * y1) + (b * y2) + (c * y3));
	}

	inline void CubicPoint(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float t, int& x, int& y)
	{
		float omt = 1 - t;

		float a = omt * omt * omt;
		float b = 3 * (omt * omt) * t;
		float c = 3 * omt * (t * t);
		float d = t * t * t;

		x = (int)((a * x1) + (b * x2) + (c * x3) + (d * x4));
		y = (int)((a * y1) + (b * y2) + (c * y3) + (d * y4));
	}
}