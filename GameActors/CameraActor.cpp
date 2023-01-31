#include "SDL_scancode.h"
#include "CameraActor.h"
#include "../Game.h"
#include "../Renderer.h"
#include "../InputSystem.h"
#include "../Components/MoveComponent.h"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// Compute new camera from this actor
	Vector3 cameraPos = GetPosition();
	Vector3 target = GetPosition() + GetForward() * 100.0f;
	Vector3 up = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const struct InputState& state)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	// wasd movement
	
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_W) == EHeld)
	{
		forwardSpeed += 150.0f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_S) == EHeld)
	{
		forwardSpeed -= 150.0f;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_A) == EHeld)
	{
		angularSpeed -= Math::TwoPi/2;
	}
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_D) == EHeld)
	{
		angularSpeed += Math::TwoPi/2;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetAngularSpeed(angularSpeed);
}