#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(Actor* owner, float mass)
	: MoveComponent(owner, mass)
	, mForwardKey(0)
	, mBackKey(0)
	, mClockwiseKey(0)
	, mCounterClockwiseKey(0)
{
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// Calculate forward speed for MoveComponent
	Vector2 force = Vector2(0.0f, 0.0f);
	if (keyState[mForwardKey])
	{
		force = Vector2::Normalize(mOwner->GetForward());
		force =  force * mMaxForwardSpeed;
		AddForce(force);
	}

	if (keyState[mBackKey])
	{
		force = Vector2::Normalize(mOwner->GetForward());
		force = force * -mMaxForwardSpeed;
		AddForce(force);
	}

	// Calculate angular speed for MoveComponent
	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}
