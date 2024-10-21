#include "Input.h"
#include <SDL.h>
#include <SDL_mouse.h>
#include <iostream>

bool Input::Initialize()
{
	int numKeys;
	const uint8_t* keyboardState = SDL_GetKeyboardState(&numKeys);

	m_keyboardState.resize(numKeys);

	// copy current keyboard state into var
	std::copy(keyboardState, keyboardState + numKeys, m_keyboardState.begin());

	m_prevKeyboardState = m_keyboardState;
	
	int x, y;
	SDL_GetMouseState(&x, &y);
	m_prevMousePosition.x = (float)x;
	m_prevMousePosition.y = (float)y;
	m_mousePosition.x = (float)x;
	m_mousePosition.y = (float)y;

	return true;
}

void Input::Shutdown()
{

}

void Input::Update()
{
	// keyboard input
	m_prevKeyboardState = m_keyboardState;
	const uint8_t* keyboardState = SDL_GetKeyboardState(nullptr);
	std::copy(keyboardState, keyboardState + m_keyboardState.size(), m_keyboardState.begin());

	// mouse input
	int x, y;
	uint32_t buttonState = SDL_GetMouseState(&x, &y);

	m_prevMousePosition = m_mousePosition;
	m_mousePosition.x = (float)x;
	m_mousePosition.y = (float)y;

	m_prevMouseButtonState = m_mouseButtonState;
	
	m_mouseButtonState[0] = buttonState & SDL_BUTTON_LMASK;
	m_mouseButtonState[1] = buttonState & SDL_BUTTON_MMASK;
	m_mouseButtonState[2] = buttonState & SDL_BUTTON_RMASK;
}