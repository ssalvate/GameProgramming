#pragma once

#include <cstddef>

#include "Component.h"

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	// Draw this mesh component
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }
protected:
	class Mesh* mMesh;
	// Which texture to use for Draw call
	size_t mTextureIndex;
};