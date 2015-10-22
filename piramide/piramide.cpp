#include "../includes/Ralha.h"


typedef Angel::vec4 point4;
typedef Angel::vec4 color4;


GLFWwindow* window;


void myDisplay( void );
void myInit( void );
void myIdle( void );

const int NumVertices = 3;


point4 eye ( 0.0, -1.0, 1.5, 1.0 );
point4 at  ( 0.0,  0.0, 0.0, 1.0 );
point4 up  ( 0.0,  1.0, 0.0, 0.0 );

mat4 modelview = LookAt( eye, at, up );
GLuint lookatmat;
GLuint projmat;
//mat4 matrizprojecao = Ortho(-2.5, 2.5, -2.5, 2.5, -1.0, 2.0);
mat4 matrizprojecao = Perspective(60, 1.0, .05, 10.0);


point4 points[6*NumVertices] = {
  // Qual é o giro?
  /*
  point4( -.5, -.5,  .5, 1 ), //P0 Base
  point4(  .5, -.5,  .5, 1 ), //P1 Base
  point4(  .5, -.5, -.5, 1 ), //P2 Base
  //
  point4(  .5, -.5, -.5, 1 ), //P2 Base
  point4( -.5, -.5, -.5, 1 ), //P3 Base
  point4( -.5, -.5,  .5, 1 ), //P0 Base
  */

  //Qual é o giro?
  point4( -.5, -.5,  .5, 1 ), //P0 Base
  point4(  .5, -.5, -.5, 1 ), //P2 Base
  point4(  .5, -.5,  .5, 1 ), //P1 Base
  point4( -.5, -.5,  .5, 1 ), //P0 Base
  point4( -.5, -.5, -.5, 1 ), //P3 Base
  point4(  .5, -.5, -.5, 1 ), //P2 Base
  //Qual é o giro?



  point4( -.5, -.5,  .5, 1 ), //P0 Face Frontal
  point4(  .5, -.5,  .5, 1 ), //P1 Face Frontal
  point4(   0,  .5,   0, 1 ), //P4 Face Frontal

  point4(  .5, -.5, -.5, 1 ), //P2 Face Posterior
  point4( -.5, -.5, -.5, 1 ), //P3 Face Posterior
  point4(   0,  .5,   0, 1 ), //P4 Face Posterior

  point4(  .5, -.5,  .5, 1 ), //P1 Lateral Direita
  point4(  .5, -.5, -.5, 1 ), //P2 Lateral Direita
  point4(   0,  .5,   0, 1 ), //P4 Lateral Direita

  point4( -.5, -.5, -.5, 1 ), //P3 Lateral Esquerda
  point4( -.5, -.5,  .5, 1 ), //P0 Lateral Esquerda
  point4(   0,  .5,   0, 1 ), //P4 Lateral Esquerda

};

color4 colors[6*NumVertices] = {
  color4( 1, 0, 0, 1 ),//red
  color4( 1, 0, 0, 1 ),//red
  color4( 1, 0, 0, 1 ),//red
  color4( 1, 0, 0, 1 ),//red
  color4( 1, 0, 0, 1 ),//red
  color4( 1, 0, 0, 1 ),//red

  color4( 0, 1, 0, 1 ),//green
  color4( 0, 1, 0, 1 ),//green
  color4( 0, 1, 0, 1 ),//green

  color4( 1, 1, 0, 1 ),//yellow
  color4( 1, 1, 0, 1 ),//yellow
  color4( 1, 1, 0, 1 ),//yellow

  color4( 0, 0, 1, 1 ),//blue
  color4( 0, 0, 1, 1 ),//blue
  color4( 0, 0, 1, 1 ),//blue

  color4( 1, 0, 1, 1 ),//magenta
  color4( 1, 0, 1, 1 ),//
  color4( 1, 0, 1, 1 ),//

};


GLfloat angulo = 0;
GLuint theta;

int main (int argc, char** argv) {
  window = inicia_glfw (3, 3, "Pirâmide Colorida girando em ZY");
  inicia_glew ( );

  myInit();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // o loop abaixo equivale ao loop de eventos da FreeGLUT
  do{
    myDisplay();
    glfwPollEvents();
    myIdle();
  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );

  // faz a faxina antes de encerrar o programa
  glfwTerminate();
  return 0;
}





void myDisplay ( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glUniform1f( theta, angulo );
  glUniformMatrix4fv( lookatmat, 1, GL_TRUE, modelview);
  glUniformMatrix4fv( projmat, 1, GL_TRUE, matrizprojecao);

  glDrawArrays( GL_TRIANGLES, 0, 6*NumVertices);

  glfwSwapBuffers(window);
}

void myIdle ( void ) {
  angulo += .25;
}

void myInit( void ) {
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
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);


  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0,
          BUFFER_OFFSET(0) );

  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET(sizeof(points)) );

  lookatmat = glGetUniformLocation( program, "mvm" );
  projmat = glGetUniformLocation( program, "proj" );
  theta = glGetUniformLocation( program, "theta");

  glEnable(GL_DEPTH_TEST);//Exercício: comente esta linha
  glEnable(GL_CULL_FACE);// e esta. O que acontece?

  glClearColor( .9, .9, .9, 1.0 );
}





























//Resposta:
  // problema com o giro da base. Habilitando o cullface ...
  // a face a ser exibida está virada para dentro da pirâmide!
  // e como esses pontos estão mais próximos temos o estranho
  // comportamento das arestas caminhantes!!!!

