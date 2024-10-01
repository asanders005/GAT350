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

private:
	SDL_Window* m_window{ nullptr };
	SDL_Renderer* m_renderer{ nullptr };
};