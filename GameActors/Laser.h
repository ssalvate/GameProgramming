#pragma once
#include "Actor.h"
#include "../Components/MoveComponent.h"

class Laser : public Actor
{
public:
	Laser(class Game* game);

	void UpdateActor(float deltaTime) override;
	void SetVelocity(Vector2 vel) { mMc->SetVelocity(vel); }

private:

	class CircleComponent* mCircle;
	class MoveComponent* mMc;
	float mDeathTimer;
};

