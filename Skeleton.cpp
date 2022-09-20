#include "Skeleton.h"
#include "AnimatedSpriteComponent.h"
#include "Game.h"

Skeleton::Skeleton(Game* game)
	: Actor(game)

{
	// Create animated sprite component
	mASC = new AnimatedSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png"),
	};

	mASC->SetAnimTextures(anims);
	mASC->SetAnimNames("Walk", std::array<int, 3>  {0, 5, 1});
	mASC->SetAnimNames("Jump", std::array<int, 3>  {6, 14, 0});
	mASC->SetAnimNames("Punch", std::array<int, 3> {15, 17, 0});
	mASC->SetCurrentAnimation("Walk");
}

void Skeleton::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
}

void Skeleton::ProcessKeyboard(const uint8_t* state)
{
	if (state[SDL_SCANCODE_SPACE]) mASC->SetCurrentAnimation("Jump");
	if (state[SDL_SCANCODE_X]) mASC->SetCurrentAnimation("Punch");

}

