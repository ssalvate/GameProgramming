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
	// Scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(48),
		static_cast<float>(48),
		1.0f
	);
	Matrix4 world = scaleMat * mOwner->GetWorldTransform();
	
	// Set world transform
	shader->SetMatrixUniform("uWorldTransform", world);
	
	// Draw quad
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