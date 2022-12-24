#pragma once
#include <cstdint>

class Component
{
public:
	// (the lower the update order, the earlier the component updates)
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// Update this component by delta time
	virtual void Update(float deltaTime);

	// Process input for this component
	virtual void ProcessInput(const uint8_t* keyState) {}
	// Called when world transform changes
	virtual void OnUpdateWorldTransform() {}

	int GetUpdateOrder()const { return mUpdateOrder; }

protected:
	// Owning Actor
	class Actor* mOwner;
	// Update order of component
	int mUpdateOrder;
};
