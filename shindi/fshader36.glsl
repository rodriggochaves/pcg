#version 130

in  vec4 color;
in  vec2 texCoord;

out vec4 fColor; 

uniform float textureexist;
uniform float colorR;
uniform float colorG;
uniform float colorB;

uniform sampler2D texture;

void main()
{
   fColor =  (texture2D( texture, texCoord ) * vec4(textureexist,textureexist,textureexist,textureexist)) + (vec4(colorR, colorG, colorB , 1 - textureexist)  * color);
}