#pragma once
#include <SDL_scancode.h>

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

// Wrapper that contains current state of input
struct InputState
{
	KeyboardState Keyboard;
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

