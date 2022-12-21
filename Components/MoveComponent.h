#pragma once

#include "Component.h"
#include "../Math.h"
#include "Component.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, float mass = 1.0f, int updateOrder = 10);

	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed)  { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	
	void AddForce(Vector3 force);
	void SetVelocity(Vector3 vel) { mVelocity = vel; }
private:
	// Controls rotation (radians/second)
	float mAngularSpeed;
	// Controls forward movement (units/second)
	float mForwardSpeed;
	
	// Newtonian physics
	Vector3 mSumOfForces;
	Vector3 mVelocity;
	float mMass;
};

