#version 330 core

in  vec4 cor;
out vec4 fColor;

void main( void ) {
   fColor = cor;
   //fColor = vec4( 0.5, 0.75, 0.05, 1.0 );
}
