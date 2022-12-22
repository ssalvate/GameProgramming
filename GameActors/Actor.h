#pragma once

#include <vector>

#include "../Math.h"

class Actor
{
public:
	enum State {
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Update all components matched to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// Any actor-specific input code (overridable)
	virtual void ActorInput(const uint8_t* keyState);

	// SetGets
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& position) { mPosition = position;mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) {mRotation = rotation; mRecomputeWorldTransform = true; }
	
	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform()const { return mWorldTransform; }

	// Transform initial forward direction <1,0,0> by the rotation quarternion
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	
	class Game* GetGame() { return mGame; }

	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
private:
	// Actors state
	State mState;
	
	// Transform
	Matrix4 mWorldTransform;
	Vector3 mPosition;		// Center position of actor
	Quaternion mRotation;   // Rotation angle in radians
	float mScale;			// Uniform scale of actor (1.0f for 100%)
	bool mRecomputeWorldTransform;

	// Components held by actor
	std::vector<class Component*> mComponents;
	class Game* mGame;
};