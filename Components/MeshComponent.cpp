#include "MeshComponent.h"
#include "../Game.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Renderer.h"
#include "../VertexArray.h"
#include "../GameActors/Actor.h"

MeshComponent::MeshComponent(Actor* owner)
	: Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		// Set active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) { t->SetActive(); }
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
}
