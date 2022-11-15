#pragma once
#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	float mLaserCooldown;
	float mRespawnCooldown;
	bool mRespawning;
	class SpriteComponent* mSc;
	class CircleComponent* mCc;
	class InputComponent* mIc;
};

