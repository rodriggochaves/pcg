#include "../includes/Ralha.h"

GLFWwindow* inicia_glfw (short int major, 
				   short int minor,
				   const char* title )
{
  GLFWwindow* window;

  if ( !glfwInit() ) {
    fprintf( stderr, "\nFalhou a iniciação da GLFW3" );
    fprintf( stderr, "\nO programa será encerrado\n" );
    exit( EXIT_FAILURE );
  }

  glfwWindowHint(GLFW_SAMPLES, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
  //O próximo hint é fundamental para MAC OS X
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //Sem o hint anterior não roda nos MAC OS X
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow( 800, 800, title, NULL, NULL);

  if( window == NULL ){
    fprintf( stderr, "Não foi possível abrir uma janela com a GLFW3.\n" );
    fprintf( stderr, "\nO programa será encerrado.\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  glfwMakeContextCurrent(window);

  //const GLubyte* versao = glGetString(GL_VERSION);
  //fprintf(stderr, "\nOpenGL Version %s\n", versao);
  return window;
}
