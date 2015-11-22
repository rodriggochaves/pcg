#version 330 core

// in vec3 ourColor;


// out vec4 color; // We set this variable in the OpenGL code.

out vec3 color;
in vec3 fragmentColor;

void main(){
	color = fragmentColor;

}

// void main( void ) {
	// a cor do triangulo vai ser vermelha
	// fColor = vec4( 1.0, 0.0, 0.0, 1.0 );
	// fColor = vec4(clamp(Position, 0.0, 1.0), 1.0);
	// color = ourColor;
	// color = vec4(ourColor, 1.0f);
// }