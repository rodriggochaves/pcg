#version 330 core

in vec4 vPosition;

void main( void ) {

   mat4 escala = mat4( .125,      0,      0,    0, 
                          0,   .125,      0,    0,
                          0,      0,   .125,    0,
                          0,      0,      0,    1 );

   gl_Position = escala * vPosition;
}
