#include "VertexArray.h"

#include <glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
{
	// Create vertex array and track ID
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// Create vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,				// The active buffer type to write to
		numVerts * 5 * sizeof(float),   // Number of bytes to copy
		verts,							// Source to copy from (pointer)
		GL_STATIC_DRAW					// How will we use this data?
		);

	// Create index buffer
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Specify vertex attrinbutes - 0Position 1Normals 2Texture
	// Position is 3 floats starting at offset 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// Attribute index (0 is first one)
		3,					// Number of components (3 in this case)
		GL_FLOAT,			// Type of components
		GL_FALSE,			// (only used for integral types)
		sizeof(float) * 8,	// Stride (usually size of each vertex)
		0);					// Offset ptr from start of vertex to this attribute
	
	// Second attribute - Normals, 3 floats
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
		reinterpret_cast<void*>(sizeof(float)*3 ));
	// Third attribute - Texture Coordinates, 2 floats
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
		reinterpret_cast<void*>(sizeof(float) * 6));
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}


void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}