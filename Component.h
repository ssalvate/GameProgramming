#pragma once
#include <cstdint>

class Component
{
public:
	// (the lower the update order, the earlier the component updates)
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update(float deltaTime);

	int GetUpdateOrder()const { return mUpdateOrder; }

	virtual void ProcessInput(const uint8_t* keyState) {}

protected:
	// Owning Actor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
};
