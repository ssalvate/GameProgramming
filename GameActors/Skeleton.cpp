#include "Skeleton.h"
#include "../Components/AnimatedSpriteComponent.h"
#include "../Game.h"

Skeleton::Skeleton(Game* game)
	: Actor(game)

{
	// Create animated sprite component
	mASC = new AnimatedSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Skeleton/Character01.png"),
		game->GetTexture("Assets/Skeleton/Character02.png"),
		game->GetTexture("Assets/Skeleton/Character03.png"),
		game->GetTexture("Assets/Skeleton/Character04.png"),
		game->GetTexture("Assets/Skeleton/Character05.png"),
		game->GetTexture("Assets/Skeleton/Character06.png"),
		game->GetTexture("Assets/Skeleton/Character07.png"),
		game->GetTexture("Assets/Skeleton/Character08.png"),
		game->GetTexture("Assets/Skeleton/Character09.png"),
		game->GetTexture("Assets/Skeleton/Character10.png"),
		game->GetTexture("Assets/Skeleton/Character11.png"),
		game->GetTexture("Assets/Skeleton/Character12.png"),
		game->GetTexture("Assets/Skeleton/Character13.png"),
		game->GetTexture("Assets/Skeleton/Character14.png"),
		game->GetTexture("Assets/Skeleton/Character15.png"),
		game->GetTexture("Assets/Skeleton/Character16.png"),
		game->GetTexture("Assets/Skeleton/Character17.png"),
		game->GetTexture("Assets/Skeleton/Character18.png"),
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

