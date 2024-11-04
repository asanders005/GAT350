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
#include "Random.h"

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void InitScene(Scene& scene);

int main(int argc, char* argv[])
{
	srand((unsigned int)time(0));

	Time time;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Ray Tracer", 1600, 900);

	Framebuffer framebuffer{ renderer, renderer.GetWidth(), renderer.GetHeight() };

	Camera camera{ 70.0f, (float)renderer.GetWidth() / renderer.GetHeight() };
	camera.SetView({ 0, 0, -5 }, { 0, 0, 0 });

	Scene scene;
	InitScene(scene);

	scene.Update();
	scene.Render(framebuffer, camera, 3, 5);

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

		framebuffer.Update();

		renderer.CopyFramebuffer(framebuffer);

		renderer.EndFrame();
	}

	return 0;
}

void InitScene(Scene& scene)
{
#pragma region MaterialCreation
	std::shared_ptr<Material> dark = std::make_shared<Metal>(color3_t{ 0.1f, 0.0f, 0.2f }, 0.0f);
	std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1.0f, 0.0f, 0.0f });
	std::shared_ptr<Material> orange = std::make_shared<Lambertian>(color3_t{ 1.0f, 0.5f, 0.0f });
	std::shared_ptr<Material> yellow = std::make_shared<Lambertian>(color3_t{ 0.85f, 1.0f, 0.0f });
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0.0f, 1.0f, 0.0f });
	std::shared_ptr<Material> turqoise = std::make_shared<Lambertian>(color3_t{ 0.0f, 0.75f, 0.75f });
	std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0.0f, 0.0f, 1.0f });
	std::shared_ptr<Material> purple = std::make_shared<Lambertian>(color3_t{ 0.35f, 0.0f, 0.75f });
	std::shared_ptr<Material> grey = std::make_shared<Lambertian>(color3_t{ 0.5f, 0.5f, 0.5f });
	std::shared_ptr<Material> Metalred = std::make_shared<Metal>(color3_t{ 1.0f, 0.0f, 0.0f }, 0.3f);
	std::shared_ptr<Material> Metalorange = std::make_shared<Metal>(color3_t{ 1.0f, 0.5f, 0.0f }, 0.1f);
	std::shared_ptr<Material> Metalyellow = std::make_shared<Metal>(color3_t{ 0.85f, 1.0f, 0.0f }, 0.3f);
	std::shared_ptr<Material> Metalgreen = std::make_shared<Metal>(color3_t{ 0.0f, 1.0f, 0.0f }, 0.1f);
	std::shared_ptr<Material> Metalturqoise = std::make_shared<Metal>(color3_t{ 0.0f, 0.75f, 0.75f }, 0.1f);
	std::shared_ptr<Material> Metalblue = std::make_shared<Metal>(color3_t{ 0.0f, 0.0f, 1.0f }, 0.3f);
	std::shared_ptr<Material> Metalpurple = std::make_shared<Metal>(color3_t{ 0.35f, 0.0f, 0.75f }, 0.1f);
	std::shared_ptr<Material> Metalgrey = std::make_shared<Metal>(color3_t{ 0.5f, 0.5f, 0.5f }, 0.5f);
	std::shared_ptr<Material> Emissivered = std::make_shared<Emissive>(color3_t{ 1.0f, 0.0f, 0.0f }, 10.0f);
	std::shared_ptr<Material> Emissiveorange = std::make_shared<Emissive>(color3_t{ 1.0f, 0.5f, 0.0f }, 7.5f);
	std::shared_ptr<Material> Emissiveyellow = std::make_shared<Emissive>(color3_t{ 0.85f, 1.0f, 0.0f }, 9.5f);
	std::shared_ptr<Material> Emissivegreen = std::make_shared<Emissive>(color3_t{ 0.0f, 1.0f, 0.0f }, 15.0f);
	std::shared_ptr<Material> Emissiveturqoise = std::make_shared<Emissive>(color3_t{ 0.0f, 0.75f, 0.75f }, 17.5f);
	std::shared_ptr<Material> Emissiveblue = std::make_shared<Emissive>(color3_t{ 0.0f, 0.0f, 1.0f }, 20.0f);
	std::shared_ptr<Material> Emissivepurple = std::make_shared<Emissive>(color3_t{ 0.35f, 0.0f, 0.75f }, 12.5f);
	std::shared_ptr<Material> Dielectricwhite = std::make_shared<Dielectric>(color3_t{ 1.0f, 1.0f, 1.0f }, 2.42f);
	std::shared_ptr<Material> Dielectricred = std::make_shared<Dielectric>(color3_t{ 1.0f, 0.0f, 0.0f }, 1.33f);
	std::shared_ptr<Material> Dielectricorange = std::make_shared<Dielectric>(color3_t{ 1.0f, 0.5f, 0.0f }, 1.33f);
	std::shared_ptr<Material> Dielectricyellow = std::make_shared<Dielectric>(color3_t{ 0.85f, 1.0f, 0.0f }, 1.33f);
	std::shared_ptr<Material> Dielectricgreen = std::make_shared<Dielectric>(color3_t{ 0.0f, 1.0f, 0.0f }, 1.33f);
	std::shared_ptr<Material> Dielectricturqoise = std::make_shared<Dielectric>(color3_t{ 0.0f, 0.75f, 0.75f }, 1.33f);
	std::shared_ptr<Material> Dielectricblue = std::make_shared<Dielectric>(color3_t{ 0.0f, 0.0f, 1.0f }, 1.33f);
	std::shared_ptr<Material> Dielectricpurple = std::make_shared<Dielectric>(color3_t{ 0.35f, 0.0f, 0.75f }, 2.42f);

	std::vector<std::shared_ptr<Material>> materials;

	materials.push_back(dark);
	materials.push_back(red);
	materials.push_back(orange);
	materials.push_back(yellow);
	materials.push_back(green);
	materials.push_back(turqoise);
	materials.push_back(blue);
	materials.push_back(purple);
	materials.push_back(grey);
	materials.push_back(Metalred);
	materials.push_back(Metalorange);
	materials.push_back(Metalyellow);
	materials.push_back(Metalgreen);
	materials.push_back(Metalturqoise);
	materials.push_back(Metalblue);
	materials.push_back(Metalpurple);
	materials.push_back(Metalgrey);
	materials.push_back(Emissivered);
	materials.push_back(Emissiveorange);
	materials.push_back(Emissiveyellow);
	materials.push_back(Emissivegreen);
	materials.push_back(Emissiveturqoise);
	materials.push_back(Emissiveblue);
	materials.push_back(Emissivepurple);
	materials.push_back(Dielectricwhite);
	materials.push_back(Dielectricred);
	materials.push_back(Dielectricorange);
	materials.push_back(Dielectricyellow);
	materials.push_back(Dielectricgreen);
	materials.push_back(Dielectricturqoise);
	materials.push_back(Dielectricblue);
	materials.push_back(Dielectricpurple);
#pragma endregion

	/*for (int i = 0; i < 10; i++)
	{
		std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(Transform{ random(glm::vec3{ -15, -2, -10 }, glm::vec3{ 15, 7.5, 30 }) }, randomf(0.5f, 3.0f), materials[random(1, materials.size())]);
		scene.AddObject(std::move(sphere));
	}*/

	/*std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(glm::vec3(0, 1, -15), 1.0f, Dielectricwhite);
	scene.AddObject(std::move(sphere));

	std::unique_ptr<Triangle> triangle = std::make_unique<Triangle>(glm::vec3(0, 2, -17), glm::vec3(2, -1, -18), glm::vec3(-2, -1, -18), Dielectricorange);
	scene.AddObject(std::move(triangle));*/

	std::unique_ptr<Model> model = std::make_unique<Model>(Transform{ { -2, 0, 5 }, { 5, 40, 0 }, glm::vec3{ 4 } }, orange);
	model->Load("models/cube.obj");
	scene.AddObject(std::move(model));

	std::unique_ptr<Plane> plane = std::make_unique<Plane>(Transform{ { 0, -2, 0 }, { 0, 0, 10} }, dark);
	scene.AddObject(std::move(plane));

	Color::SetBlendMode(BlendMode::NORMAL);
}