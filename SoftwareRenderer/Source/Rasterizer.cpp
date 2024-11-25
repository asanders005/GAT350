#include "Rasterizer.h"
#include "Framebuffer.h"

namespace Rasterizer
{
	void Triangle(Framebuffer& framebuffer,
		const glm::vec2& p0,
		const glm::vec2& p1,
		const glm::vec2& p2,
		const vertex_output_t& v0,
		const vertex_output_t& v1,
		const vertex_output_t& v2)
	{
		// compute bounding box	
		int xmin = (int)std::max<float>(0, std::min({ std::floor(p0.x), std::floor(p1.x), std::floor(p2.x) }));
		int xmax = (int)std::min<float>((float)framebuffer.m_width - 1, std::max({ std::floor(p0.x), std::floor(p1.x), std::floor(p2.x) }));
		int ymin = (int)std::max<float>(0, std::min({ std::floor(p0.y), std::floor(p1.y), std::floor(p2.y) }));
		int ymax = (int)std::min<float>((float)framebuffer.m_height - 1, std::max({ std::floor(p0.y), std::floor(p1.y), std::floor(p2.y) }));

		for (int y = ymin; y <= ymax; ++y) 
		{
			for (int x = xmin; x <= xmax; ++x) 
			{
				glm::vec2 p{ x, y };

				// compute barycentric coordinates
				
				// this gives us twice the signed area of the whole triangle using the cross product
				float area = Math::Cross(p1 - p0, p2 - p0);
				// the sign tells us triangle winding (clockwise/counterclockwise)
				if (std::abs(area) < std::numeric_limits<float>::epsilon()) return;

				// area of subtriangles divided by total area
				float w0 = Math::Cross(p1 - p, p2 - p) / area;	// area of subtriangle opposite to v0
				float w1 = Math::Cross(p2 - p, p0 - p) / area;	// area of subtriangle opposite to v1
				float w2 = 1.0f - w0 - w1;					// area of subtriangle opposite to v2

				if (w0 >= 0 && w1 >= 0 && w2 >= 0) 
				{
					// interpolate vertex attributes
					
					// create fragment shader input
					fragment_input_t fragment;
					
					// call fragment shader
					color4_t output_color = FragmentShader::Process(fragment);
					framebuffer.DrawPoint(x, y, Color::ColorConvert(output_color));
				}
			}
		}
	}

}