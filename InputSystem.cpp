#include "InputSystem.h"
#include "SDL.h"
#include "Math.h"

bool InputSystem::Initialize()
{
    // Assign current state pointer
    mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
    // Clear previous state memory, set each elem(512) to 0 
    memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

    return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
    // Copy current state to previous
        // Keyboard
    memcpy(
        mState.Keyboard.mPrevState,
        mState.Keyboard.mCurrState,
        SDL_NUM_SCANCODES
    );

        //Mouse
    mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
}

void InputSystem::Update()
{
    // Mouse
    int x = 0, y = 0;
    mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
    mState.Mouse.mMousePos.x = static_cast<float>(x);
    mState.Mouse.mMousePos.y = static_cast<float>(y);

}

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
    return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
    if (mPrevState[keyCode] == 0) {
        if (mCurrState[keyCode] == 0) return ENone;
        else return EPressed;
    }
    else // Prev state is 1
    {
        if (mCurrState[keyCode] == 0) return EReleased;
        else return EHeld;
    }
}

bool MouseState::GetButtonValue(int button) const
{
    return (SDL_BUTTON(button) & mCurrButtons) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
    int mask = SDL_BUTTON(button);
    if ((mask & mPrevButtons) == 0)
    {
        if ((mask & mCurrButtons) == 0) return ENone;
        else return EPressed;
    }
    else // Prev state is 1
    {
        if ((mask & mCurrButtons) == 0) return EReleased;
        else return EHeld;
    }
}
