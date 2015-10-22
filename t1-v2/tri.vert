#version 330 core

layout (location = 0) in  vec4 vPosition;
layout (location = 1) in  vec4 vColor;

out vec4 cor;

uniform mat4 mvm;
uniform mat4 proj;
uniform float theta;

void main( void ) {
   // Computa o seno e cosseno de theta

   float angle = radians( theta );
   float c = cos( angle );
   float s = sin( angle );


   //--------------------------------------------------------------
   // Matrizes de rotação

   mat4 ry = mat4( c,   0.0,  0.0,   0.0,
                   0.0, 1.0,  0.0, 0.0,
                   0.0, 0.0,  c,   0.0,
                   0.0, 0.0,  0.0, 1.0 );



   gl_Position =  proj  * mvm *  ry * vPosition;
   //gl_Position =  proj  * mvm * vPosition;

   cor = vColor;
}

