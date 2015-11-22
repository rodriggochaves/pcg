#version 330 core

// layout (location = 0) in vec3 position;
// layout (location = 1) in vec3 color;
// out vec3 ourColor;


// in vec4 vPosition;
// uniform mat4 Model;
// uniform mat4 View;
// uniform mat4 Projection;

// void main( void ) {
//    // gl_Position = vPosition; 
//    gl_Position = vec4(position, 1.0);
//    ourColor = color;
// }

layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

void main() {
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;

	fragmentColor = vertexColor;
}