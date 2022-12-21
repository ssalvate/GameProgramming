#include "MoveComponent.h"
#include "../GameActors/Actor.h"

MoveComponent::MoveComponent(class Actor* owner,  float mass, int updateOrder)
	:Component(owner, updateOrder)
	, mVelocity(Vector3::Zero)
	, mSumOfForces(Vector3::Zero)
	, mMass(mass)
{}

void MoveComponent::Update(float deltaTime)
{
	// Rotation
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		// Create Quaternion for incremental rotation
		// (Rotate about up axis)
		Quaternion inc(Vector3::UnitZ, angle);
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	
	Vector3 acceleration = mSumOfForces * (1.0f/ mMass);
	// Clear forces for impulse potential
	mSumOfForces = Vector3::Zero;
	// (Semi-Implicit) Euler Integration
	// Update velocity
	mVelocity = mVelocity + acceleration * deltaTime;
	// Update position
	Vector3 pos = mOwner->GetPosition();
	pos += mVelocity * deltaTime;
		
	// Wrap around screen - Asteroids specific
	if (pos.x < -512.0f) { pos.x = 512.0f; }
	else if (pos.x > 512.0f) { pos.x = -512.0f; }
	if (pos.y < -384.0f) { pos.y = 384.0f; }
	else if (pos.y > 384.0f) { pos.y = -384.0f; }
		
	mOwner->SetPosition(pos);

}

void MoveComponent::AddForce(Vector3 force)
{
	// Add the magnitude to the sum of forces
	mSumOfForces += force;
}
