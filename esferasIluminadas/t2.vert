#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;


out vec4 vColor;

// Output data ; will be interpolated for each fragment.
// Traduzindo: Gouraud lighting 
// ou seja, a iluminação é feita no vertex shader
// O resultado não é realmente impressionante.
// Vide página 79 do PDF SIGGRAPH 2013 ou ainda
// página 292 do livro texto

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;

// Eu separei a Model da View
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec4 LightPosition;
uniform float Shininess;


void main(){
  vec4 vp = View * Model * vec4( vPosition, 1.0 );
  // Transformando a posição do vértice para as coordenadas de câmera
  vec3 pos = vp.xyz;

  vec3 L = normalize( LightPosition.xyz - pos );
  vec3 E = -normalize( pos );
  vec3 H = normalize( L + E );

  // Traga a normal para coordenadas de câmera
  // Importante: 0.0 pois a normal É um VETOR
  // Lembre que Model, View são matrizes 4x4 e por isso 
  // vNormal tem que ser redimensionada como um VETOR
  vec3 N = normalize( vec3( View * Model * vec4(vNormal, 0.0) ) );

  // Computando os termos da equação de Iluminação
  vec4 ambient = AmbientProduct;

  float Kd = max( dot( L, N ), 0.0 );
  vec4 diffuse = Kd * DiffuseProduct;

  float Ks = pow( max( dot( N, H ), 0.0 ), Shininess );
  vec4 specular = Ks * SpecularProduct;

  if ( dot( L, N) < 0.0 ) specular = vec4( 0.0, 0.0, 0.0, 1.0 );

  gl_Position = Projection * View * Model * vec4(vPosition, 1.0);

  vColor = ambient + diffuse + specular;
  vColor.a = 1.0;
}

