#ifndef __RALHA_H__
#define __RALHA_H__

#include <stdlib.h>//incluida pelo Ralha
#include <stdio.h>//incluida pelo Ralha

#include "./Angel.h"
#include <GLFW/glfw3.h>
//#include "/opt/local/include/GLFW/glfw3.h"

GLFWwindow* inicia_glfw (short int major, 
			 short int minor,
			 const char* title );

void inicia_glew ( void );


#endif // __RALHA_H__
