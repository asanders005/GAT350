#include "Renderer.h"

#include <SDL.h>
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize();
	renderer->CreateWindow("window", 800, 600);

	while (true)
	{
		renderer->BeginFrame();

		renderer->EndFrame();
	}
	
	return 0;
}