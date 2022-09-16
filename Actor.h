#pragma once

#include <vector>

#include "Math.h"

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

	// SetGets
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& position) { mPosition = position; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) {mRotation = rotation; }

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
	Vector2 mPosition; // Center position of actor
	float mScale;      // Uniform scale of actor (1.0f for 100%)
	float mRotation;   // Rotation angle in radians

	// Components held by actor
	std::vector<class Component*> mComponents;
	class Game* mGame;

};