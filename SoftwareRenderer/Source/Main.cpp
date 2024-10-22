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
	scenery.Load("Scenery.jpg");

	Image eevee;
	eevee.Load("Eevee.png");

	Image pokeball;
	pokeball.Load("image.png");

	Image imageAlpha;
	imageAlpha.Load("colors.png");
	Post::Alpha(imageAlpha.m_buffer, 64);

	Color::SetBlendMode(BlendMode::NORMAL);

	//vertices_t vertices{ {-5, -5, 0}, {5, 5, 0}, {-5, 5, 0} };
	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->Load("fox.obj");
	model->SetColor({ 255, 128, 0, 255 });

	std::vector<std::unique_ptr<Actor>> actors;

	for (int i = 0; i < 5; i++)
	{
		Transform transform{ {randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f)}, {0, randomf(-90, 90), 0}, glm::vec3{4}};
		std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
		actor->SetColor({ (uint8_t)random(0, 255), (uint8_t)random(0, 255), (uint8_t)random(0, 255), (uint8_t)random(0, 255) });
		actors.push_back(std::move(actor));
	}

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

#pragma region Images
		/*Color::SetBlendMode(BlendMode::NORMAL);
		framebuffer->DrawImage(700, 450, scenery);*/
		/*Color::SetBlendMode(BlendMode::ALPHA);
		framebuffer->DrawImage(360, 250, eevee);*/
		//Color::SetBlendMode(BlendMode::ADDITIVE);
		//framebuffer->DrawImage(mx, my, pokeball);
		//Color::SetBlendMode(BlendMode::MULTIPLY);
		//framebuffer->DrawRect(10, 390, 200, 200, { 180, 25, 255, 128 });

#pragma endregion

#pragma region Primitives
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

		//framebuffer->DrawLinearCurve(50, 50, 100, 100, { 255, 255, 255, 255 });
		////framebuffer->DrawQuadraticCurve(100, 50, 200, 100, mx, my, 10, { 255, 255, 255, 255 });
		//framebuffer->DrawCubicCurve(200, 100, 250, 200, mx, my, 300, 50, 50, { 255, 255, 255, 255 });

		//int x, y;
		//Math::CubicPoint(200, 100, 250, 200, mx, my, 300, 50, t, x, y);
		//framebuffer->DrawRect(x - 20, y - 20, 40, 40, { 180, 0, 255, 255 });

		/*framebuffer->DrawLine(-80, 5, 600, 40, { 0, 255, 25, 255 });
		framebuffer->DrawLine(80, 150, 40, 275, { 0, 255, 25, 255 });
		framebuffer->DrawRect(20, 20, 100, 100, { 0, 255, 180 });
		framebuffer->DrawTriangle(200, 50, 50, 295, 375, 110, { 255, 120, 0 });
		framebuffer->DrawCircle(225, 125, 50, { 0, 0, 255 });*/

#pragma endregion

#pragma region postProcess
		//Post::Invert(framebuffer->m_buffer);
		//Post::Monochrome(framebuffer->m_buffer);
		//Post::Brightness(framebuffer->m_buffer, 40);
		//Post::Noise(framebuffer->m_buffer, 100);
		//Post::Threshold(framebuffer->m_buffer, 125);

		//Post::BoxBlur(framebuffer->m_buffer, framebuffer->m_width, framebuffer->m_height);
		//Post::Sharpen(framebuffer->m_buffer, framebuffer->m_width, framebuffer->m_height);
		//Post::Edge(framebuffer->m_buffer, framebuffer->m_width, framebuffer->m_height, 120);
		//Post::Emboss(framebuffer->m_buffer, framebuffer->m_width, framebuffer->m_height);
		//Post::EmbossColor(framebuffer->m_buffer, framebuffer->m_width, framebuffer->m_height);
		

		/*Post::ColorBalance(framebuffer->m_buffer, 12, -7, 12);
		Post::GaussianBlur(framebuffer->m_buffer, framebuffer->m_width, framebuffer->m_height);
		Post::Posterize(framebuffer->m_buffer, 8);*/

#pragma endregion
		
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

			cameraTransform.rotation.y += input->GetMouseRelative().x * 0.5f;
			cameraTransform.rotation.x += input->GetMouseRelative().y * 0.5f;
			cameraTransform.rotation.x = Math::Clamp(cameraTransform.rotation.x, -89.99f, 89.99f);

			glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

			cameraTransform.position += offset * 50.0f * time->GetDeltaTime();
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