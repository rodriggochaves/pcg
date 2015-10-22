#include "Ralha.h"


typedef Angel::vec4 point4;
typedef Angel::vec4 color4;


point4 points[3] = {
  point4( -0.5, -0.5, 0.0, 1.0 ),
  point4(  0.5, -0.5, 0.0, 1.0 ),
  point4(  0.0,  0.5, 0.0, 1.0 )
};


GLfloat Theta[3] = { 0.0, 0.5, 0.0 };
GLuint theta; // The location of the "theta" shader uniform variable


GLFWwindow* window;


void myInit( void ) {

  srand(time(NULL));

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "rct.vert", "rct.frag" );
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

  theta = glGetUniformLocation( program, "theta" );

  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}




void myDisplay (void ) {
    glClear( GL_COLOR_BUFFER_BIT );
    glUniform3fv( theta, 1, Theta);
    glDrawArrays( GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
}

void myIdle ( void ) {
  static  GLfloat angle = rand()%90;
  Theta[0] = sin(angle);
  Theta[1] = cos(angle);
  Theta[2] = Theta[1]+Theta[0];
  angle += 0.05;
 }

int main (int argc, char** argv) {
  window = inicia_glfw (3, 3, "Triângulo Camaleão");
  inicia_glew ( );
 
  myInit();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
  // o loop abaixo equivale ao loop de eventos da Freeglut
  do{
    myDisplay();
    glfwPollEvents();
    myIdle ();
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );

  // faz a faxina antes de encerrar o programa
  glfwTerminate();
  return 0;
}



