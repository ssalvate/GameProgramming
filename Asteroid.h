#pragma once
#include "Actor.h"

class Asteroid :
    public Actor
{
public:
	Asteroid(class Game* game);
private:
	class CircleComponent* mCircle;
};

