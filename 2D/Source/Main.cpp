#include "Renderer.h"
#include "Framebuffer.h"

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize();
	renderer->CreateWindow("window", 800, 600);

	std::unique_ptr<Framebuffer> framebuffer = std::make_unique<Framebuffer>(*renderer.get(), 400, 300);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		renderer->BeginFrame();

		framebuffer->Clear({ 0, 0, 0, 255 });

		for (int i = 0; i < 1000; i++)
		{
			int x = rand() % 400;
			int y = rand() % 300;
			framebuffer->DrawPoint(x, y, { 180, 0, 255, 255 });
		}

		framebuffer->DrawRect(20, 20, 100, 100, { 0, 255, 180 });

		framebuffer->Update();

		renderer->CopyFramebuffer(*framebuffer.get());

		renderer->EndFrame();
	}
	
	return 0;
}