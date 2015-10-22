// Rodrigo de Araujo Chaves
// 13/0132624

#include "../includes/Ralha.h"
#include <math.h>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

GLFWwindow *window;

void myDisplay ( void );
void myInit ( void );
void myIdle( void );

const int NumVertices = 9600;//com 9600 ainda dá pra ver a falta

point4 points[NumVertices];

color4 colors = ( 1.0, 0.0, 0.0, 1.0 );

void circulo (point4 pontos[], const int numvert)
{
	GLfloat ang = 0.0; // angulo em radianos

	GLfloat incr = (2 * M_PI) / (numvert - 1);

	pontos[0] = point4( 0.0, 0.0, 1.0, 1.0 ); //centro do circulo

	for ( int i = 1; i<numvert; i++ )	{
		pontos[i] = point4( cos(ang),  sin(ang), 0.0, 1.0 );
		ang = ang + incr;
	}
}

point4 eye ( 0.0, -1.0, 1.5, 1.0 );
point4 at  ( 0.0,  0.0, 0.0, 1.0 );
point4 up  ( 0.0,  1.0, 0.0, 0.0 );

mat4 modelview = LookAt( eye, at, up );
GLuint lookatmat;
GLuint projmat;
// mat4 matrizprojecao = Ortho(-2.5, 2.5, -2.5, 2.5, -1.0, 2.0);
// mat4 matrizprojecao = Perspective(60, 1.0, .05, 10.0); -> original
mat4 matrizprojecao = Perspective(60, 1.0, .05, 10.0);
GLfloat angulo = 0;
GLuint theta;

int main(int argc, char const *argv[])
{
	window = inicia_glfw(3, 3, "Trabalho 1");
	inicia_glew();

	myInit();

	// Garante que a tecla 'esc' pode ser usada para fechar a janela
  	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do
	{
		myDisplay();
		myIdle();
		glfwPollEvents();
	} while ( glfwGetKey( window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose( window ) == 0);

	glfwTerminate();

	return 0;
}


void myDisplay ( void ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1f( theta, angulo );
  	glUniformMatrix4fv( lookatmat, 1, GL_TRUE, modelview);
  	glUniformMatrix4fv( projmat, 1, GL_TRUE, matrizprojecao);

	// Gera a imagem com triângulos
	// glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  	glFrontFace(GL_CCW);   /* default */
  	//glFrontFace(GL_CW);
	glDrawArrays( GL_TRIANGLE_FAN, 0, NumVertices );

	glfwSwapBuffers( window );
}

void myIdle ( void ) {
	angulo += .25;
}

void myInit ( void ) {
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
  	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

  	// set up vertex arrays
  	// GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	// glEnableVertexAttribArray( vPosition );
  	// glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  	lookatmat = glGetUniformLocation( program, "mvm" );
  	projmat = glGetUniformLocation( program, "proj" );
  	theta = glGetUniformLocation( program, "theta");

  	// set up vertex array ( layout = 0 )
  	glEnableVertexAttribArray( 0 );
  	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );

	glEnable(GL_DEPTH_TEST);
	// esconde o lado interno do cone
	// glEnable(GL_CULL_FACE);

	glClearColor( 1.0, 1.0, 1.0, 1.0 );

}