#pragma once
#include <SDL_scancode.h>
#include "Math.h"

enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};

class KeyboardState {
public:
	// Friend so InputSystem can easily update it
	friend class InputSystem;

	// Get just the boolean true/false value of key
	bool GetKeyValue(SDL_Scancode keyCode) const;

	// Get a state based on current and previous frame
	ButtonState GetKeyState(SDL_Scancode keyCode) const;
private:
	// Current state
	const Uint8* mCurrState;
	// State previous frame
	Uint8 mPrevState[SDL_NUM_SCANCODES];
};

class MouseState
{
public:
	friend class InputSystem;

	// For mouse position
	const Vector2& GetPosition() const { return mMousePos; }

	// For buttons
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;
private:
	// Store mouse position
	Vector2 mMousePos;
	// Store button data
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
};

// Wrapper that contains current state of input
struct InputState
{
	KeyboardState Keyboard;
	MouseState Mouse;
};

class InputSystem
{
public:
	bool Initialize();
	void Shutdown();

	// Called right before SDL_PollEvents loop
	void PrepareForUpdate();
	// Called right after SDL_PollEvents loop
	void Update();

	const InputState& GetState() const { return mState; }
private:
	InputState mState;
};

