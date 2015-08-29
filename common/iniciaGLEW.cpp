#include "../includes/Ralha.h"


void inicia_glew ( void ) 
{
#ifdef __APPLE__
  return;
#else
  glewExperimental=true; // Needed in core profile 

  if ( glewInit() != GLEW_OK ) {
    fprintf(stderr, "\nFailed to initialize GLEW\n");
    fprintf( stderr, "\nO programa será encerrado\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
#endif
}
/*
Para o MAC OS X não é necessário o uso da GLEW. Para que o código
seja portável, fizemos o teste e caso esteja em uma máquina APPLE
a função simplesmente retorna sem nada fazer. 
Para as demais plataformas, a GLEW é iniciada.

 */
