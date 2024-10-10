#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize();
	renderer->CreateWindow("window", 800, 600);

	std::unique_ptr<Framebuffer> framebuffer = std::make_unique<Framebuffer>(*renderer.get(), 800, 600);

	Image scenery;
	scenery.Load("Scenery.jpg");
	Image eevee;
	eevee.Load("Eevee.png");
	Image pokeball;
	pokeball.Load("image.png");

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

		framebuffer->DrawImage(-10, -10, scenery);
		framebuffer->DrawImage(100, 10, eevee);

		//for (int i = 0; i < 3; i++)
		//{
		//	int x = rand() % 1000 - 100;
		//	int y = rand() % 800 - 100;
		//	/*int x2 = rand() % 400;
		//	int y2 = rand() % 300;*/

		//	//framebuffer->drawpoint(x, y, { 180, 0, 255, 255 });
		//	//framebuffer->DrawLine(x, y, x2, y2, { 0, 255, 25, 255 });
		//	framebuffer->DrawImage(x, y, pokeball);
		//}	

		//int mx, my;
		//SDL_GetMouseState(&mx, &my);

		//framebuffer->DrawLinearCurve(50, 50, 100, 100, { 255, 255, 255, 255 });
		////framebuffer->DrawQuadraticCurve(100, 50, 200, 100, mx, my, 10, { 255, 255, 255, 255 });
		//framebuffer->DrawCubicCurve(200, 100, 250, 200, mx, my, 300, 50, 50, { 255, 255, 255, 255 });

		//int ticks = SDL_GetTicks();
		//float time = ticks * 0.001f;
		//float t = std::abs(std::sin(time));
		//int x, y;
		//Math::CubicPoint(200, 100, 250, 200, mx, my, 300, 50, t, x, y);
		//framebuffer->DrawRect(x - 20, y - 20, 40, 40, { 180, 0, 255, 255 });

		/*framebuffer->DrawLine(-80, 5, 600, 40, { 0, 255, 25, 255 });
		framebuffer->DrawLine(80, 150, 40, 275, { 0, 255, 25, 255 });
		framebuffer->DrawRect(20, 20, 100, 100, { 0, 255, 180 });
		framebuffer->DrawTriangle(200, 50, 50, 295, 375, 110, { 255, 120, 0 });
		framebuffer->DrawCircle(225, 125, 50, { 0, 0, 255 });*/

		//Post::Invert(framebuffer->m_buffer);
		//Post::Monochrome(framebuffer->m_buffer);
		Post::ColorBalance(framebuffer->m_buffer, 12, -7, 12);
		//Post::Brightness(framebuffer->m_buffer, 40);
		//Post::Noise(framebuffer->m_buffer, 100);
		//Post::Threshold(framebuffer->m_buffer, 125);
		Post::Posterize(framebuffer->m_buffer, 8);

		framebuffer->Update();

		renderer->CopyFramebuffer(*framebuffer.get());

		renderer->EndFrame();
	}

	return 0;
}