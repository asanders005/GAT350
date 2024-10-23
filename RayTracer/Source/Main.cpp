#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"
#include "ETime.h"
#include "Transform.h"
#include "Input.h"
#include "Camera.h"
#include "Tracer.h"

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	std::unique_ptr<Time> time = std::make_unique<Time>();

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize();
	renderer->CreateWindow("Ray Tracer", 1600, 900);

	std::unique_ptr<Framebuffer> framebuffer = std::make_unique<Framebuffer>(*renderer.get(), renderer->GetWidth(), renderer->GetHeight());

	Camera camera{ 70.0f, (float)renderer->GetWidth() / renderer->GetHeight() };
	camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

	Tracer tracer;

	Color::SetBlendMode(BlendMode::NORMAL);

	bool quit = false;
	while (!quit)
	{
		time->Tick();

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

		//renderer->BeginFrame();

		framebuffer->Clear(Color::ColorConvert(color3_t{ 0.35f, 0, 0.65f }));

		tracer.Render(*framebuffer.get(), camera);

		framebuffer->Update();

		renderer->CopyFramebuffer(*framebuffer.get());

		renderer->EndFrame();
	}

	return 0;
}