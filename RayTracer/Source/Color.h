#pragma once
#include "MathUtils.h"
#include <glm/glm.hpp>
#include <SDL.h>
#include <algorithm>

using color_t = SDL_Color;
using color3_t = glm::vec3;
using color4_t = glm::vec4;


//blending
enum class BlendMode
{
	NORMAL,
	ALPHA,
	ADDITIVE,
	MULTIPLY
};

namespace Color {
	inline color_t ColorConvert(const color4_t& color4)
	{
		color_t color;

		color.r = (uint8_t)(Math::Clamp(color4.r, 0.0f, 1.0f) * 255);
		color.g = (uint8_t)(Math::Clamp(color4.g, 0.0f, 1.0f) * 255);
		color.b = (uint8_t)(Math::Clamp(color4.b, 0.0f, 1.0f) * 255);
		color.a = (uint8_t)(Math::Clamp(color4.a, 0.0f, 1.0f) * 255);

		return color;
	}

	inline color_t ColorConvert(const color3_t& color3)
	{
		color_t color;

		color.r = (uint8_t)(Math::Clamp(color3.r, 0.0f, 1.0f) * 255);
		color.g = (uint8_t)(Math::Clamp(color3.g, 0.0f, 1.0f) * 255);
		color.b = (uint8_t)(Math::Clamp(color3.b, 0.0f, 1.0f) * 255);
		color.a = 255;

		return color;
	}

	inline color4_t ColorConvert(const color_t& color)
	{
		color4_t color4;

		color4.r = (Math::Clamp(color.r, (uint8_t)0, (uint8_t)255) / 255.0f);
		color4.g = (Math::Clamp(color.g, (uint8_t)0, (uint8_t)255) / 255.0f);
		color4.b = (Math::Clamp(color.b, (uint8_t)0, (uint8_t)255) / 255.0f);
		color4.a = (Math::Clamp(color.a, (uint8_t)0, (uint8_t)255) / 255.0f);

		return color4;
	}

	color_t NormalBlend(const color_t& src, const color_t& dst);
	color_t AlphaBlend(const color_t& src, const color_t& dst);
	color_t AdditiveBlend(const color_t& src, const color_t& dst);
	color_t MultiplyBlend(const color_t& src, const color_t& dst);

	void SetBlendMode(BlendMode blendMode);
	color_t ColorBlend(const color_t& src, const color_t& dst);
}