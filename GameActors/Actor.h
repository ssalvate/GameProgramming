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
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& position) { mPosition = position;mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) {mRotation = rotation; mRecomputeWorldTransform = true; }
	
	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform()const { return mWorldTransform; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation),Math::Sin(mRotation)); }

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
	Vector2 mPosition; // Center position of actor
	float mScale;      // Uniform scale of actor (1.0f for 100%)
	float mRotation;   // Rotation angle in radians
	bool mRecomputeWorldTransform;

	// Components held by actor
	std::vector<class Component*> mComponents;
	class Game* mGame;
};