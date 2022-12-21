// Request GLSL 3.3
#version 330

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
// Simple view-projection == top-right coord-> 512,384
// bottom-left-> -512,-384 , when window is 1024x768

// Attribute 0 is position, 1 is normal, 2 is tex coords
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

// Texture coordinates for output
out vec2 fragTexCoord;

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	// Transform position to world space, then clip space
	gl_Position = pos * uWorldTransform * uViewProj;
	// Pass along the texture corrdinate to frag shader
	fragTexCoord = inTexCoord;
}