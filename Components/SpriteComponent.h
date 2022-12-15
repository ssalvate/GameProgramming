#pragma once

#include "Component.h"
#include "SDL.h"

class SpriteComponent : public Component 
{
public:
	// Lower draw order corresponds with further back
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder()  const { return mDrawOrder; }
	int GetTexWidth()  const { return mTexWidth;  }
	int GetTexHeight() const { return mTexHeight; }

protected:
	class Texture* mTexture;
	int mDrawOrder;
	int mTexWidth;
	int mTexHeight;
};