#include "./includes/Ralha.h"


typedef Angel::vec4 point4;
typedef Angel::vec4 color4;


void myDisplay( GLuint program );
GLuint myInit( void );

GLfloat points[] = {
        // Positions         // Colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top 
    };

// point4 points[3] = {
// 	point4( -0.5, -0.5, 0.0, 1.0 ),
// 	point4(  0.5, -0.5, 0.0, 1.0 ),
// 	point4(  0.0,  0.5, 0.0, 1.0 )
// };

//variaveis que vão cuidar das tranformações em MVP
mat4 projection, view, model;

GLFWwindow *window;


int main (int argc, char** argv) {
	//inicializa janela em window
	window = inicia_glfw(3, 3, "Trabalho Final");

	// inicializa o programa a partir dos shaders
	GLuint program = myInit();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
  	do {
  		// muda o display
		myDisplay(program);

		// puxa eventos da pilha da OPENGL
		glfwPollEvents();

		//limpa a tela para o próximo loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  	} // Check if the ESC key was pressed or the window was closed
  	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );

  	/* Before  your application  exits,  you should  terminate the  GLFW
     library if it has been initialized. */
  	glfwTerminate();
}


void myDisplay( GLuint program ) {
	// garante que estamos usando a janela "window"
	glfwMakeContextCurrent(window);

	//garante que estamos usando o programa "program"
	glUseProgram(program);

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	glfwSwapBuffers(window);
}


GLuint myInit( void ) {
	// Load shaders and use the resulting shader program
	// Cria o programa a partir dos sharders passados
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
	// carrega a posição do triangulo no buffer
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

	// set up vertex arrays 
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, 
	      BUFFER_OFFSET(0) );

	// Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

  	// configura um plano de fundo branco
  	glClearColor( 1.0, 1.0, 1.0, 1.0 );

  	return program;
}