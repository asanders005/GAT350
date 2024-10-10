#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>

namespace Post
{
	void Invert(std::vector<color_t>& buffer)
	{
		/*for (auto& color : buffer)
		{
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
		}*/

		std::for_each(buffer.begin(), buffer.end(), [](auto& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});
	}

	void Monochrome(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](auto& color)
			{
				uint8_t avg = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

				color.r = avg;
				color.g = avg;
				color.b = avg;
			});
	}

	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{
		std::for_each(buffer.begin(), buffer.end(), [ro, go, bo](auto& color)
			{
				color.r = static_cast<uint8_t>(Math::Clamp(color.r + ro, 0, 255));
				color.g = static_cast<uint8_t>(Math::Clamp(color.g + go, 0, 255));
				color.b = static_cast<uint8_t>(Math::Clamp(color.b + bo, 0, 255));
			});
	}

	void Brightness(std::vector<color_t>& buffer, int brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](auto& color)
			{
				color.r = static_cast<uint8_t>(Math::Clamp(color.r + brightness, 0, 255));
				color.g = static_cast<uint8_t>(Math::Clamp(color.g + brightness, 0, 255));
				color.b = static_cast<uint8_t>(Math::Clamp(color.b + brightness, 0, 255));
			});
	}

	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{
		std::for_each(buffer.begin(), buffer.end(), [noise](auto& color)
			{
				color.r = static_cast<uint8_t>(Math::Clamp(color.r + Math::Random(-noise, noise), 0, 255));
				color.g = static_cast<uint8_t>(Math::Clamp(color.g + Math::Random(-noise, noise), 0, 255));
				color.b = static_cast<uint8_t>(Math::Clamp(color.b + Math::Random(-noise, noise), 0, 255));
			});
	}

	void Threshold(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](auto& color)
			{
				uint8_t avg = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

				if (avg >= threshold)
				{
					color.r = 255;
					color.g = 255;
					color.b = 255;
				}
				else
				{
					color.r = 0;
					color.g = 0;
					color.b = 0;
				}
			});
	}
    void Posterize(std::vector<color_t>& buffer, uint8_t levels)
    {
		uint8_t level = 255 / levels;

		std::for_each(buffer.begin(), buffer.end(), [level](auto& color)
			{
				color.r = (color.r / level) * level;
				color.g = (color.g / level) * level;
				color.b = (color.b / level) * level;
			});
    }
}