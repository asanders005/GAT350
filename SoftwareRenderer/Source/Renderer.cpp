#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>

bool Renderer::Initialize()
{
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
		return false;
	}

    return true;
}

bool Renderer::CreateWindow(std::string name, int width, int height)
{
	// create window
	// returns pointer to window if successful or nullptr if failed
	m_width = width;
	m_height = height;

	m_window = SDL_CreateWindow(name.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_width, m_height,
		SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	// create renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    return true;
}

void Renderer::BeginFrame()
{
	// clear screen
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(m_renderer);
}

void Renderer::EndFrame()
{
	//SDL_WarpMouseInWindow(m_window, m_width / 2, m_height / 2);
	// show screen
	SDL_RenderPresent(m_renderer);
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
	SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}

void Renderer::operator = (const Framebuffer& framebuffer)
{
	SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}
