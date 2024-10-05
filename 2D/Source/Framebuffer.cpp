#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
	if (!m_texture)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}

	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	//x = (x < 0) ? 0 : (x >= m_width) ? m_width : x;
	//y = (y < 0) ? 0 : (y >= m_height) ? m_height : y;

	m_buffer[x + (y * m_width)] = color;
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	// calculate deltas
	int dx = x2 - x1;
	int dy = y2 - y1;

	// check for steep line
	bool steep = std::abs(dy) > std::abs(dx);
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	// ensure left to right drawing
	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	// recalculate deltas
	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	// set error term and y step direction
	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	// clip line
	int code1 = ComputeRegionCode(x1, y1);
	int code2 = ComputeRegionCode(x2, y2);
	bool valid = false;

	while (true)
	{
		if (code1 == 0 && code2 == 0)
		{
			// Both endpoints are inside the boundary
			valid = true;
			break;
		}
		else if (code1 & code2)
		{
			// Both endpoints are outside the same boundary
			break;
		}
		else
		{
			// At least one endpoint is outside the boundary
			int codeOut = 0;
			int x{ 0 }, y{ 0 };

			// Use region code to choose endpoint to clip
			if (code1 != 0) codeOut = code1; // start point is outside
			else codeOut = code2; // end point is outside

			// Find intersection point;
			// will be different for each boundary
			if (codeOut & TOP)
			{
				x = x1 + (x2 - x1) * (0 - y1) / (y2 - y1);
				y = 0;
			}
			else if (codeOut & BOTTOM)
			{
				x = x1 + (x2 - x1) * (m_height - y1) / (y2 - y1);
				y = m_height - 1;
			}
			else if (codeOut & LEFT)
			{
				y = y1 + (y2 - y1) * (0 - x1) / (x2 - x1);
				x = 0;
			}
			else if (codeOut & RIGHT)
			{
				y = y1 + (y2 - y1) * (m_width - x1) / (x2 - x1);
				x = m_width - 1;
			}

			// Move outside point to intersection point
			if (codeOut == code1)
			{
				x1 = x;
				y1 = y;
				code1 = ComputeRegionCode(x1, y1);
			}
			else 
			{
				x2 = x;
				y2 = y;
				code2 = ComputeRegionCode(x2, y2);
			}
		}
	}

	// draw line points
	if (valid)
	{
		for (int x = x1, y = y1; x <= x2; x++)
		{
			(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
			error -= dy;

			// update error term

			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}
	}
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx == 0)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y <= y2; y++)
		{
			m_buffer[x1 + y * m_width] = color;
		}
		return;
	}
	
	float m = (float)dy / dx;
	float b = y1 - (m * x1);

	if (std::abs(dx) > std::abs(dy))
	{
		if (x1 > x2) std::swap(x1, x2);
		for (int x = x1; x <= x2; x++)
		{
			// y = mx + b
			int y = (int)round((m * x) + b);
			m_buffer[x + y * m_width] = color;
		}
	}
	else
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y <= y2; y++)
		{
			// x = (y - b) / m
			int x = (int)round((y - b) / m);
			m_buffer[x + y * m_width] = color;
		}
	}
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	if (x + w < 0 || x >= m_width || y + h < 0 || y >= m_height) return;
	
	int x1 = std::max(x, 0);
	int x2 = std::min(x + w, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + h, m_height);

	for (int sy = y1; sy < y2; sy++)
	{
		int index = x1 + (sy * m_width);
		std::fill(m_buffer.begin() + index, m_buffer.begin() + (index + x2 - x1), color);
		//for (int sx = x1; sx < x2; sx++)
		//{
		//	//m_buffer[sx + (sy * m_width)] = color;
		//}
	}
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawCircle(int xc, int yc, int r, const color_t& color)
{
	int x = 0, y = r;
	int d = 3 - (2 * r);

	DrawOctant(xc, yc, x, y, color);

	while (y >= x)
	{
		// check for decision parameter and update d, y accordingly
		if (d > 0)
		{
			y--;
			d +=  4 * (x - y) + 10;
		}
		else d += (4 * x) + 6;

		// increment x after updating decision parameter
		x++;

		// Clip Circle
		if (x < 0 || x >= m_width || y < 0 || y >= m_height) continue;
		// Draw circle using new coordinates
		DrawOctant(xc, yc, x, y, color);
	}
}

void Framebuffer::DrawOctant(int xc, int yc, int x, int y, const color_t& color)
{
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

int Framebuffer::ComputeRegionCode(int x, int y)
{
	int code = INSIDE;

	if (x < 0) code |= LEFT;
	else if (x >= m_width) code |= RIGHT;
	if (y < 0) code |= TOP;
	else if (y >= m_height) code |= BOTTOM;

	return code;
}
