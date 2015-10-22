#version 330 core

in  vec4 vPosition;
out vec4 vColor;

uniform vec3 theta;

void main( void ) {

   gl_Position = vPosition; 

   vColor = vec4(theta, 1.0);
}
