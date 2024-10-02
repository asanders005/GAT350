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
	x = (x < 0) ? 0 : (x >= m_width) ? m_width : x;
	y = (y < 0) ? 0 : (y >= m_height) ? m_height : y;

	m_buffer[x + (y * m_width)] = color;
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	x = (x < 0) ? 0 : (x >= m_width) ? m_width : x;
	y = (y < 0) ? 0 : (y >= m_height) ? m_height : y;

	for (int i = 0; i < h; i++)
	{
		int yPos = y + i;
		if (yPos < 0 || yPos >= m_height) continue;
		for (int k = 0; k < w; k++)
		{
			int xPos = x + k;
			if (xPos < 0 || xPos >= m_width) continue;
			m_buffer[xPos + (yPos * m_width)] = color;
		}
	}
}
