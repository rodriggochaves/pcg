#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{    
    // color = texture(texture_diffuse1, TexCoords);
    vec3 I = normalize(Position - cameraPos);
    //refletir a imagem utilizada na skybox, tornando-se um espelho :D
    vec3 R = reflect(I, normalize(Normal));
    color = texture(skybox, R);
}