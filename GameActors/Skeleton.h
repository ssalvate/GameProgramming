#pragma once
#include "Actor.h"
class Skeleton : public Actor
{
public:
	Skeleton(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* mState);
private:
	class AnimatedSpriteComponent* mASC;
};