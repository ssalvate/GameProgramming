// Request GLSL 3.3
#version 330

// ----  INPUTS   ----
// Tex coord from vertex shader
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// ----  OUTPUTS  ----
// Output color
out vec4 outColor;

// ----  UNIFORMS ----
// For texture sampling
uniform sampler2D uTexture;

// Create a struct for directionl light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;
};

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Ambient light level
uniform vec3 uAmbientLight;
// Specular power for this surface
uniform float uSpecPower;

// Directional light (only 1 for now)
uniform DirectionalLight uDirLight;

void main()
{
	// Surface normal
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if(NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	// Final color is texture color times phong light (alpha = 1)
	outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}