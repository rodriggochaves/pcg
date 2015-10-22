#version 330 core

/* 
   As coordenadas  de vPosition são expressos em valores adotados no  
   Espaço de Modelagem do Objeto
 */

layout(location = 0) in vec3 vPosition; 

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main(){

  gl_Position =  Projection * View * Model * vec4(vPosition,1);
	
}

