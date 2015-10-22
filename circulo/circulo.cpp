#include "../includes/Ralha.h"
#include <math.h>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;



void myDisplay( void );
void myInit( void );


const int NumVertices = 100;//com 9600 ainda dá pra ver a falta

point4 points[NumVertices];


void circulo (point4 pontos[], const int numvert)
{
  GLfloat ang = 0.0; // angulo em radianos

  GLfloat incr = (2 * M_PI) / (numvert - 1);

  pontos[0] = point4( 0.0, 0.0, 0.0, 1.0 ); //centro do circulo

  for ( int i = 1; i<numvert; i++ )
    {
      pontos[i] = point4( cos(ang),  sin(ang), 0.0, 1.0 );
      ang = ang + incr;
    }
 }



GLFWwindow* window;


int main (int argc, char** argv) {
  window = inicia_glfw (3, 3, "Limite n->00");
  inicia_glew ( );

  myInit();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // o loop abaixo equivale ao loop de eventos da FreeGLUT
  do{
    myDisplay();
    glfwPollEvents();
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );

  // faz a faxina antes de encerrar o programa
  glfwTerminate();
  return 0;
}





void myDisplay( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);   /* default */
  //glFrontFace(GL_CW);
  glDrawArrays( GL_TRIANGLE_FAN, 0, NumVertices);

  glfwSwapBuffers(window);
}


void myInit( void ) {
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "./tri.vert", "./tri.frag" );
  glUseProgram( program );

  circulo (points, NumVertices);

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initalize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

  // set up vertex array ( layout = 0 )
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );


  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}


