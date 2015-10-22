#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "Angel.h"

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

void mydisplay( void );
void init( void );
void idle( void);


point4 points[3] = {
  point4( -0.5, -0.5, 0.0, 1.0 ),
  point4(  0.5, -0.5, 0.0, 1.0 ),
  point4(  0.0,  0.5, 0.0, 1.0 )
};




GLfloat angulo = 0.0;
GLuint theta; // The location of the "theta" shader uniform variable


int main (int argc, char** argv) {
  glutInit( &argc, argv);
  glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
  glutInitWindowSize( 800, 800 );
  glutCreateWindow( "Triangulo" );

  glutDisplayFunc( mydisplay );
  glutIdleFunc( idle );

  glewInit();

  init();

  glutMainLoop();

}


void mydisplay( void ) {
  glClear( GL_COLOR_BUFFER_BIT );

  glUniform1f( theta, angulo);

  glDrawArrays( GL_TRIANGLES, 0, 3);

  glFlush();
}


void idle ( void ) {
  angulo += .025;

  glutPostRedisplay();
}



void init( void ) {

  srand(time(NULL));

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

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, 
          BUFFER_OFFSET(0) );

  theta = glGetUniformLocation( program, "theta" );

  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}
