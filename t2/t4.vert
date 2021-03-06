#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 texturecoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 LightPosition;

out vec3 N;
out vec3 L;
out vec3 E;
out vec2 UV;

void main(){
  gl_Position = Projection * View * Model * vec4(vPosition, 1.0);

  // Não esqueça de trazer a Normal para as coordenadas de câmera!
  N = (View * Model * vec4(vNormal, 0.0)).xyz;

  L = LightPosition.xyz - vPosition;
  if ( LightPosition.w == 0 ) L = LightPosition.xyz;
  //E = vPosition.xyz; <== oh, aqui o erro!!!!
  E = (View * Model * vec4(vPosition, 0.0)).xyz;

  UV = texturecoord;
}

