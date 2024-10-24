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
#include "Actor.h"
#include "Random.h"

#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	std::unique_ptr<Time> time = std::make_unique<Time>();

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize();
	renderer->CreateWindow("window", 1400, 900);

	std::unique_ptr<Input> input = std::make_unique<Input>();
	input->Initialize();
	input->Update();

	std::unique_ptr<Camera> camera = std::make_unique<Camera>(renderer->GetWidth(), renderer->GetHeight());
	camera->SetView({ 0, 0, -50 }, glm::vec3{ 0 });
	camera->SetProjection(60.0f, (float)renderer->GetWidth() / renderer->GetHeight(), 0.1f, 200.0f);
	Transform cameraTransform{ { 0, 0, -50 } };

	std::unique_ptr<Framebuffer> framebuffer = std::make_unique<Framebuffer>(*renderer.get(), renderer->GetWidth(), renderer->GetHeight());

	Image scenery;
	scenery.Load("Shrine.jpg");

	Color::SetBlendMode(BlendMode::NORMAL);

	//vertices_t vertices{ {-5, -5, 0}, {5, 5, 0}, {-5, 5, 0} };
	std::shared_ptr<Model> fox = std::make_shared<Model>();
	fox->Load("fox.obj");
	fox->SetColor({ 255, 128, 0, 255 });
	
	std::shared_ptr<Model> gate = std::make_shared<Model>();
	gate->Load("gate.obj");
	gate->SetColor({ 255, 128, 0, 255 });
	
	std::shared_ptr<Model> tree = std::make_shared<Model>();
	tree->Load("tree.obj");
	tree->SetColor({ 255, 128, 0, 255 });

	std::vector<std::unique_ptr<Actor>> actors;

	Transform transform{ { 0, 0, 0 }, { 0, -75, 0 }, glm::vec3{5}};
	std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, fox);
	actor->SetColor({ 255, 128, 0, 255 });
	actors.push_back(std::move(actor));
	
	transform = { { -20, 7, 25 }, { 0, -45, 0 }, glm::vec3{5}};
	actor = std::make_unique<Actor>(transform, gate);
	actor->SetColor({ 255, 28, 28, 255 });
	actors.push_back(std::move(actor));

	transform = { { 25, -5, 40 }, {5, 75, 5}, glm::vec3{0.25f} };
	actor = std::make_unique<Actor>(transform, tree);
	actor->SetColor({ 255, 0, 255, 255 });
	actors.push_back(std::move(actor));

	bool quit = false;
	while (!quit)
	{
		time->Tick();
		input->Update();

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

		framebuffer->Clear({ 0, 0, 0, 255 });

		int mx, my;
		SDL_GetMouseState(&mx, &my);

		Color::SetBlendMode(BlendMode::NORMAL);
		framebuffer->DrawImage(700, 450, scenery);
		Post::ColorBalance(framebuffer->m_buffer, -20, -20, 10);
		Post::Brightness(framebuffer->m_buffer, -80);
		
		if (input->GetMouseButtonDown(2))
		{
			input->SetRelativeMode(true);

			glm::vec3 direction{ 0 };
			if (input->GetKeyDown(SDL_SCANCODE_D)) direction.x += 1;
			if (input->GetKeyDown(SDL_SCANCODE_A)) direction.x -= 1;
			if (input->GetKeyDown(SDL_SCANCODE_E)) direction.y += 1;
			if (input->GetKeyDown(SDL_SCANCODE_Q)) direction.y -= 1;
			if (input->GetKeyDown(SDL_SCANCODE_W)) direction.z += 1;
			if (input->GetKeyDown(SDL_SCANCODE_S)) direction.z -= 1;

			cameraTransform.rotation.y += input->GetMouseRelative().x * 0.1f;
			cameraTransform.rotation.x += input->GetMouseRelative().y * 0.1f;
			cameraTransform.rotation.x = Math::Clamp(cameraTransform.rotation.x, -89.99f, 89.99f);

			glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

			cameraTransform.position += offset * 15.0f * time->GetDeltaTime();
		}
		else
		{
			input->SetRelativeMode(false);
		}
		
		camera->SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());

		//transform.rotation += time->GetDeltaTime() * 135;
		for (auto& actor : actors)
		{
			actor->Draw(*framebuffer.get(), *camera.get());
		}

		framebuffer->Update();

		renderer->CopyFramebuffer(*framebuffer.get());

		renderer->EndFrame();
	}

	input->Shutdown();

	return 0;
}