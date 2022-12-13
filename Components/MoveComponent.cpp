#include "MoveComponent.h"
#include "../GameActors/Actor.h"

MoveComponent::MoveComponent(class Actor* owner,  float mass, int updateOrder)
	:Component(owner, updateOrder)
	, mVelocity(Vector2::Zero)
	, mSumOfForces(Vector2::Zero)
	, mMass(mass)
{}

void MoveComponent::Update(float deltaTime)
{
	// Rotation
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	
	Vector2 acceleration = mSumOfForces * (1.0f/ mMass);
	// Clear forces for impulse potential
	mSumOfForces = Vector2::Zero;
	// (Semi-Implicit) Euler Integration
	// Update velocity
	mVelocity = mVelocity + acceleration * deltaTime;
	// Update position
	Vector2 pos = mOwner->GetPosition();
	pos += mVelocity * deltaTime;
		
	// Wrap around screen - Asteroids specific
	if (pos.x < -512.0f) { pos.x = 512.0f; }
	else if (pos.x > 512.0f) { pos.x = -512.0f; }
	if (pos.y < -384.0f) { pos.y = 384.0f; }
	else if (pos.y > 384.0f) { pos.y = -384.0f; }
		
	mOwner->SetPosition(pos);

}

void MoveComponent::AddForce(Vector2 force)
{
	// Add the magnitude to the sum of forces
	mSumOfForces += force;
}
