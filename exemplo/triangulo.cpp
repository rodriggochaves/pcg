#include "../includes/Ralha.h"

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

void myDisplay( void );
void myInit( void );


point4 points[3] = {
  point4( -0.5, -0.5, 0.0, 1.0 ),
  point4(  0.5, -0.5, 0.0, 1.0 ),
  point4(  0.0,  0.5, 0.0, 1.0 )
};



GLFWwindow *window;


int main (int argc, char** argv) {
  window = inicia_glfw(3, 3, "Meu 1º Triângulo");
  inicia_glew ( );


  myInit();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
  do{
    myDisplay();
    glfwPollEvents();
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );

  /* Before  your application  exits,  you should  terminate the  GLFW
     library if it has been initialized. */
  glfwTerminate();
}






void myDisplay( void ) {
  glClear( GL_COLOR_BUFFER_BIT );

  glDrawArrays( GL_TRIANGLES, 0, 3 );

  glfwSwapBuffers(window);
}


void myInit( void ) {
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "./tri.vert", "./tri.frag" );
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

  // set up vertex arrays 
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, 
          BUFFER_OFFSET(0) );


  glClearColor( 1.0, 1.0, 1.0, 1.0 );

}




