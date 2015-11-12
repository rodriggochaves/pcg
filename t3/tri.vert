#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 ourColor;


in vec4 vPosition;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main( void ) {
   // gl_Position = vPosition; 
   gl_Position = vec4(position, 1.0);
   ourColor = color;
}
