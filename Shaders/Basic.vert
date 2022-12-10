// Request GLSL 3.3
#version 330

// Any vertex attributes go here
// for now, just a position
in vec3 inPosition;

void main()
{
	// Directly pass along inPosition to gl_Position
	gl_Position = vec4(inPosition, 1.0);
}