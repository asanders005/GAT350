#pragma once
#include <SDL.h>
#include <string>

class Renderer
{
public:
	bool Initialize();
	bool CreateWindow(std::string name, int width, int height);

	void BeginFrame();
	void EndFrame();

	void CopyFramebuffer(const class Framebuffer& framebuffer);

	void operator = (const class Framebuffer& framebuffer);

	friend class Framebuffer;

private:
	SDL_Window* m_window{ nullptr };
	SDL_Renderer* m_renderer{ nullptr };

	int m_width = 0;
	int m_height = 0;
};