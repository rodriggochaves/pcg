#version 130

in vec4 vPosition;
in vec4 vColor;
in  vec2 vTexCoord;
out vec4 color;
out vec2 texCoord;


uniform mat4 mvm;
uniform mat4 rotacaom;
uniform mat4 posicaom;
uniform float posx;
uniform float posy;

void main()
{
   color = vColor;
    texCoord    = vTexCoord;
   gl_Position =  mvm * posicaom * rotacaom * vPosition  ;
}