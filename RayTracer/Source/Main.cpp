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
#include "Scene.h"

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	Time time;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", 1600, 900);

	Framebuffer framebuffer{ renderer, renderer.GetWidth(), renderer.GetHeight() };

	Camera camera{ 70.0f, (float)renderer.GetWidth() / renderer.GetHeight() };
	camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

	Scene scene;

	std::shared_ptr<Material> material = std::make_shared<Material>(color3_t{ 0.35f, 0.0f, 0.75f });
	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(glm::vec3{ 0 }, 10.0f, material);
	
	scene.AddObject(std::move(sphere));

	Color::SetBlendMode(BlendMode::NORMAL);

	bool quit = false;
	while (!quit)
	{
		time.Tick();

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

		framebuffer.Clear(Color::ColorConvert(color3_t{ 0.25f, 1.0f, 0.25f }));

		scene.Render(framebuffer, camera);

		framebuffer.Update();

		renderer.CopyFramebuffer(framebuffer);

		renderer.EndFrame();
	}

	return 0;
}