// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// Output color
out vec4 outColor;

// For texture sampling
uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
	outColor = texture(uTexture, fragTexCoord);
}