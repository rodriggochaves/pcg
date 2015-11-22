#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

in vec3 TexCoords;
out vec4 color;
// out vec3 color;

// Values that stay constant for the whole mesh.
// uniform sampler2D myTextureSampler;
uniform samplerCube skybox;

void main() {

	// Output color = color of the texture at the specified UV
	// color = texture( myTextureSampler, UV ).rgb;
	color = texture(skybox, TexCoords);
}
