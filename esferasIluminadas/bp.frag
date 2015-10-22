#version 330 core

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;

in vec3 N;
in vec3 L;
in vec3 E;

out vec4 out_Color;

void main ( void )
{
  vec3 NN = normalize( N );
  vec3 EE = normalize( E );
  vec3 LL = normalize( L );

  vec4 ambient, diffuse, specular;

  vec3 H = normalize( LL + EE );
  float Kd = max( dot( LL, NN ), 0.0 );
  float Ks = pow( max( dot( NN, H ), 0.0 ), Shininess );

  ambient = AmbientProduct;
  diffuse = Kd * DiffuseProduct;

  if ( dot( LL, NN ) < 0.0 ) specular = vec4( 0.0, 0.0, 0.0, 1.0 );
  else specular = Ks * SpecularProduct;

  //gl_FragColor color is drecreated or even removed
  out_Color = vec4( (ambient + diffuse + specular).xyz, 1.0 );
}

/*
Notem  que normalizamos  os vetores  no  fragment shader  ao invez  de
faze-lo no vertex shader. Se tivéssemos feito a normalização no vertex
shader, não poderíamos garantir que as normais interpoladas produzidas
pelo  rasterizador  viriam  a   ter  a  magnitude  necessária  para  a
computação da iluminação.

Este shader,  bem como  o seu correspondente  vertex shader,  estão na
página 296 do livro texto.
 */
