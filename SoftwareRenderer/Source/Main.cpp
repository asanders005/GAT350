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
#include "Shader.h"

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

	Color::SetBlendMode(BlendMode::NORMAL);

	//Shader
	VertexShader::uniforms.view = camera->GetView();
	VertexShader::uniforms.projection = camera->GetProjection();
	VertexShader::uniforms.ambient = color3_t{ 0.15f, 0.02f, 0.35f };

	VertexShader::uniforms.light.position = glm::vec3{ 0, 10, -10 };
	VertexShader::uniforms.light.direction = glm::vec3{ 0, -1, 0 };
	VertexShader::uniforms.light.color = color3_t{ 1 };

	Shader::framebuffer = framebuffer.get();

	//vertices_t vertices{ {-5, -5, 0}, {5, 5, 0}, {-5, 5, 0} };
	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->Load("Models/bunny.obj");
	std::shared_ptr<Model> model2 = std::make_shared<Model>();
	model2->Load("Models/dragon.obj");

	std::shared_ptr<material_t> material = std::make_shared<material_t>();
	material->albedo = color3_t{ 0.5f, 0.25f, 0.75f };
	material->specular = color3_t{ 1 };
	material->shininess = 8.0f;

	std::shared_ptr<material_t> material2 = std::make_shared<material_t>();
	material2->albedo = Color::HSVtoRGB({ 132, 1.0f, 0.66f });
	material2->specular = color3_t{ 0.0f, 0.9f, 0.6f };
	material2->shininess = 32.0f;

	std::vector<std::unique_ptr<Actor>> actors;

	Transform transform{ glm::vec3{ -10, -5, 0 }, glm::vec3{ 90, -135, 0 }, glm::vec3{ 5 } };
	std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model, material);
	actors.push_back(std::move(actor));
	
	transform = Transform{ glm::vec3{ 10, -2.5f, 0 }, glm::vec3{ 0, -25, 0 }, glm::vec3{ 1.5f } };
	std::unique_ptr<Actor> actor2 = std::make_unique<Actor>(transform, model2, material2);
	actors.push_back(std::move(actor2));

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
		VertexShader::uniforms.view = camera->GetView();

		//transform.rotation += time->GetDeltaTime() * 135;
		for (auto& actor : actors)
		{
			actor->Draw();
		}

		framebuffer->Update();

		renderer->CopyFramebuffer(*framebuffer.get());

		renderer->EndFrame();
	}

	input->Shutdown();

	return 0;
}