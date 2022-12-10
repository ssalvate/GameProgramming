#include "SpriteComponent.h"
#include "../Shader.h"
#include "../Game.h"
#include "../GameActors/Actor.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder )
	:Component(owner)
	,mDrawOrder(drawOrder)
	,mTexture(nullptr)
	,mTexWidth(0)
	,mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	glDrawElements(
		GL_TRIANGLES,	// Type of polugon/primitive to draw
		6,				// Number of indices in index buffer
		GL_UNSIGNED_INT,// Type of each index
		nullptr			// Usually nullptr
	);
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}