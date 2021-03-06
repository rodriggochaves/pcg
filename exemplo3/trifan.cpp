#include "Ralha.h"



typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

void mydisplay( void );
void init( void );

point4 points[4] = {
  point4( -1.5, -1.5, -1.5, 1.0 ),
  point4(  1.5, -1.5, 0.0, 1.0 ),
  point4(  1.5,  1.5, 0.0, 1.0 ),
  point4( -1.5,  1.5, 1.5, 1.0 ),
};

GLFWwindow* window;

int main (int argc, char** argv) {
  window = inicia_glfw(3, 3, "Quadrado ==  Leque Triangular");
  inicia_glew();

  init();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
  do{
    mydisplay();
    // Swap buffers
   
    glfwPollEvents();
 
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );

  glfwTerminate();
}



void mydisplay( void ) {
  glClear( GL_COLOR_BUFFER_BIT );

  //glDrawArrays( GL_TRIANGLE_FAN, 0, 4);
  glDrawArrays( GL_TRIANGLE_FAN, 0, 4);
  glfwSwapBuffers(window);
}


void init( void ) {
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "tri.vert", "tri.frag" );
  glUseProgram( program );

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initalize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

  // set up vertex array 
  // layout (location = 0) in vec4 vPosition
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 
          BUFFER_OFFSET(0) );

  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}
