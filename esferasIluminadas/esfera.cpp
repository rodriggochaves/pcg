#include <vector>
/* Ordem das inclusões é importante!
   A classe C++ vector é usada em objloader.cpp
   Por isso, a inclusão de vector tem que preceder a inclusão de objloader.h
*/
#include "../includes/Ralha.h"
#include <SOIL/SOIL.h>

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

#include "../common/objloader.hpp"


GLFWwindow *window, *janela, *xanela, *fenestra;

using namespace std; //não precisaremos escrever std::vector<vec3> vertices;

GLuint program;
GLuint VAO, VAO1, VAO2, VAO3;

GLuint vb, uvb, nb;

GLfloat angulo = 0.0f;

vector<vec3> vertices, normals;
vector<vec2> uvs;

mat4 projection, view, model;
GLuint projectionLoc, viewLoc, modelLoc;
GLuint textureLoc, texture;

GLuint myInit_noLights ( GLFWwindow* fenetre, const char* vs, const char* fs )
{
  glfwMakeContextCurrent(fenetre);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( vs, fs );
  glUseProgram(program);

  // define a cor de fundo da janela: fundo ciano
  glClearColor( 0.0, 1.0, 1.0, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);


  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Read our .obj file
  bool res = loadOBJ("esfera.obj", vertices, uvs, normals);

  // Neste shader não vamos usar texturas e tão pouco normais
  // já que queremos mostrar como a falta de luz influencia no
  // resultado final

  // carregue o vetor de coordenadas de vértices,
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3),
	       &vertices[0], GL_STATIC_DRAW);

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0); //associado ao layout(location = 0)
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


  return program;
}

// faz dobradinha como o myInit_noLights

void myDisplay1 ( GLuint sp1 )
{
  glfwMakeContextCurrent(window);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram( sp1 );

  // Vamos Rodar a esfera em Y e encolhe-la em 50%
  // A rotação não será percebida!! Por que?
  model = Scale(.5) * RotateY(1.5*angulo);
  /* Mesmo encolhendo a metade, por inspeção no .obj, eu sei que
     partes da esfera ficarão fora do cubo de modelagem canônica.
     E como vou usar projeção Ortográfica, então eu desloco a
     câmera 2 unidades no sentido positivo (ou o mundo, 2 unidades
     no sentido oposto---vide abaixo)
     Matriz de Visualização
  */
  view = Translate( 0, 0, -2);
  // a matriz de projeção abaixo corresponde a uma escala do modelo.
  projection = Ortho(-5,5,-5,5,-5,5);

  // Recuperando os locais das variáveis uniformes das matrizes
  modelLoc  = glGetUniformLocation( sp1, "Model" );
  viewLoc  = glGetUniformLocation( sp1, "View" );
  projectionLoc = glGetUniformLocation( sp1, "Projection" );
  // Passando as matrizes para o vertex shader em sp1
  glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
  glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
  glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );

  // Desenhe a esfera
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  glfwSwapBuffers(window);

  glUseProgram( 0 );
}

/*   *************************************************  */


GLuint myInit_LightsOn ( GLFWwindow* fenetre, const char* vs, const char* fs )
{
  glfwMakeContextCurrent(fenetre);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( vs, fs );
  glUseProgram(program);

  // define a cor de fundo da janela: fundo ciano
  glClearColor(0.0, 1.0, 1.0, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &VAO1); // VAO1 é uma variável global
  glBindVertexArray(VAO1);

  // não é necessário reler o arquivo  .obj
  bool res = loadOBJ("esfera.obj", vertices, uvs, normals);

  // carregue normals num VBO

  glGenBuffers(1, &nb);
  glBindBuffer(GL_ARRAY_BUFFER, nb);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3),
	       &normals[0], GL_STATIC_DRAW);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


  //2nd attribute buffer : Normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, nb);
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


  //ILUMINAÇÂO

  // Initialize shader lighting parameters
  point4 light_position( 0.0, 0.0, 5.0, 0.0 );
  color4 light_ambient(  0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse(  0.8, 0.8, 0.8, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

  color4 material_ambient(  1.0, 1.0, 1.0, 1.0 );
  color4 material_diffuse(  1.0, 0.8, 0.0, 1.0 );
  color4 material_specular( 1.0, 1.0, 1.0, 1.0 );
  float  material_shininess = 50.0;

  color4 ambient_product  = light_ambient * material_ambient;
  color4 diffuse_product  = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;


  glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product );
  glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product );
  glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product );

  glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		1, light_position );

  glUniform1f( glGetUniformLocation(program, "Shininess"),
	       material_shininess );


  return program;
}




void myDisplay2 ( GLuint sp2 )
{
  glfwMakeContextCurrent(janela);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram( sp2 );

  // Retrieve transformation uniform variable locations ( sp2 )
  modelLoc  = glGetUniformLocation( sp2, "Model" );
  viewLoc = glGetUniformLocation( sp2, "View" );
  projectionLoc = glGetUniformLocation( sp2, "Projection" );
  // Vou usar os mesmos valores para as matrizes
  // O que se espera é ter a diferença produzida tão apenas pela iluminação
  model = Scale(.5) * RotateY(1.5*angulo);
  view = Translate( 0, 0, -2);
  projection = Ortho(-5,5,-5,5,-5,5);
  // Passando as matrizes para o vertex shader em sp1
  glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
  glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
  glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );

  // Desenhe a esfera
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  glfwSwapBuffers(janela);

  glUseProgram( 0 );
}

/* ********************************************************** */

GLuint myInit_BlinnPhong ( GLFWwindow* fenetre, const char* vs, const char* fs )
{
  glfwMakeContextCurrent(fenetre);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( vs, fs );
  glUseProgram(program);

  // define a cor de fundo da janela: fundo ciano
  glClearColor(0.0, 1.0, 1.0, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &VAO2); // VAO1 é uma variável global
  glBindVertexArray(VAO2);


  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


  //2nd attribute buffer : Normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, nb);
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );


  //ILUMINAÇÂO

  // Initialize shader lighting parameters
  point4 light_position( 0.0, 0.0, 5.0, 0.0 );
  color4 light_ambient(  0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse(  0.8, 0.8, 0.8, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

  color4 material_ambient(  1.0, 1.0, 1.0, 1.0 );
  color4 material_diffuse(  1.0, 0.8, 0.0, 1.0 );
  color4 material_specular( 1.0, 1.0, 1.0, 1.0 );
  float  material_shininess = 50.0;

  color4 ambient_product  = light_ambient * material_ambient;
  color4 diffuse_product  = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;


  glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product );
  glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product );
  glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product );

  glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		1, light_position );

  glUniform1f( glGetUniformLocation(program, "Shininess"),
	       material_shininess );


  return program;
}

void myDisplay3 ( GLuint sp3 )
{
  glfwMakeContextCurrent(xanela);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram( sp3 );

  // Retrieve transformation uniform variable locations ( sp3 )
  modelLoc  = glGetUniformLocation( sp3, "Model" );
  viewLoc  = glGetUniformLocation( sp3, "View" );
  projectionLoc = glGetUniformLocation( sp3, "Projection" );
  // Vou usar os mesmos valores para as matrizes
  // O que se espera é ter a diferença produzida tão apenas pela iluminação
  model = Scale(.5) * RotateY(1.5*angulo);
  view = Translate( 0, 0, -2);
  projection = Ortho(-5,5,-5,5,-5,5);
  // Passando as matrizes para o vertex shader em sp1
  glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
  glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
  glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );

  // Desenhe a esfera
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  glfwSwapBuffers(xanela);

  glUseProgram( 0 );
}

/******************************************************/
/*    4ª Janela: Blinn Phong Ilumination plus Texture */
/******************************************************/
GLuint myInit_IlumTexture(GLFWwindow* fenetre, const char* vs, const char* fs)
{
  glfwMakeContextCurrent(fenetre);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( vs, fs );
  glUseProgram(program);

  // define a cor de fundo da janela: fundo ciano
  glClearColor(0.0, 1.0, 1.0, 1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &VAO3);
  glBindVertexArray(VAO3);

  // precisamos carregar as coordenadas de textura na GPU
  glGenBuffers(1, &uvb);
  glBindBuffer(GL_ARRAY_BUFFER, uvb);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2),
	       &uvs[0], GL_STATIC_DRAW);

  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // 2nd attribute buffer : UVs (coord de textura)
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, uvb);
  glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  //3rd attribute buffer : Normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, nb);
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  /* **************** manipulando as texturas********************* */
  /*****************************************************************/
  glGenTextures( 1, &texture );
  glBindTexture(GL_TEXTURE_2D, texture);

  int width, height;
  unsigned char* imagem =
    SOIL_load_image("./planet_Quom1200.png", &width, &height, 0, SOIL_LOAD_RGB);
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, imagem );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

  SOIL_free_image_data(imagem);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(textureLoc, 0);

  /* **************** manipulando as texturas********************* */
  /*****************************************************************/


  //ILUMINAÇÂO

  // Initialize shader lighting parameters
  point4 light_position( 0.0, 0.0, 5.0, 0.0 );
  color4 light_ambient(  0.2, 0.2, 0.2, 1.0 );
  color4 light_diffuse(  0.8, 0.8, 0.8, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

  color4 material_ambient(  1.0, 1.0, 1.0, 1.0 );
  color4 material_diffuse(  1.0, 0.8, 0.0, 1.0 );
  color4 material_specular( 1.0, 1.0, 1.0, 1.0 );
  float  material_shininess = 50.0;

  color4 ambient_product  = light_ambient * material_ambient;
  color4 diffuse_product  = light_diffuse * material_diffuse;
  color4 specular_product = light_specular * material_specular;


  glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product );
  glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product );
  glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product );

  glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		1, light_position );

  glUniform1f( glGetUniformLocation(program, "Shininess"),
	       material_shininess );


  return program;
}

void myDisplay4 ( GLuint sp4 )
{
  glfwMakeContextCurrent(fenestra);

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram( sp4 );

  // Retrieve transformation uniform variable locations ( sp3 )
  modelLoc  = glGetUniformLocation( sp4, "Model" );
  viewLoc = glGetUniformLocation( sp4, "View" );
  projectionLoc = glGetUniformLocation( sp4, "Projection" );
  // Vou usar os mesmos valores para as matrizes
  // O que se espera é ter a diferença produzida tão apenas pela iluminação
  model = Scale(.5) * RotateY(1.5*angulo);
  view = Translate( 0, 0, -2);
  projection = Ortho(-5,5,-5,5,-5,5);
  // Passando as matrizes para o vertex shader em sp1
  glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
  glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
  // Notem que view é na verdade a modelview
  glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );


  // Desenhe a esfera
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  glfwSwapBuffers(fenestra);

  glUseProgram( 0 );
}


/******************************************************/
/******************************************************/
int main( void )
{
  if ( !glfwInit() )
    {
      fprintf( stderr, "\nFalhou a iniciação da GLFW3" );
      fprintf( stderr, "\nO programa não executará\n" );
      exit( EXIT_FAILURE );
    }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //O próximo hint é fundamental para MAC OS X
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window =  glfwCreateWindow( 600, 600, "Esfera sem iluminação", NULL, NULL);

  if( window == NULL ){
    fprintf( stderr, "Falha na abertura de janela GLFW.\n" );
    fprintf( stderr, "\nO programa será encerrado\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  janela = glfwCreateWindow( 600, 600, "Gouraud Lighting", NULL, window);

  if( janela == NULL){
    fprintf( stderr, "Falha na abertura da 2ª janela GLFW.\n" );
    fprintf( stderr, "\nO programa será encerrado\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  xanela = glfwCreateWindow( 600, 600, "Blinn Fong Lighting", NULL, window);

  if( xanela == NULL){
    fprintf( stderr, "Falha na abertura da 3ª janela GLFW.\n" );
    fprintf( stderr, "\nO programa será encerrado\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  fenestra = glfwCreateWindow( 600, 600, "Blinn Fong Lighting com Textura",
			       NULL, window);

  if( fenestra == NULL){
    fprintf( stderr, "Falha na abertura da 4ª janela GLFW.\n" );
    fprintf( stderr, "\nO programa será encerrado\n" );
    glfwTerminate();
    exit( EXIT_FAILURE );
  }

  glfwSetWindowPos(janela, 600, 0);
  glfwSetWindowPos(xanela, 1200, 0);
  glfwSetWindowPos(fenestra, 0, 600);

  glfwMakeContextCurrent(window);
  inicia_glew();

  GLuint sp1, sp2, sp3, sp4;//nomes dos Shaders Programs
  sp1 = myInit_noLights(window, "t1.vert", "sl.frag");

  // Iluminação no vertex shader aka Gouraud Shading
  // Notem que o fragment shader é apenas um "passe adiante"

  sp2 = myInit_LightsOn(janela, "t2.vert", "gl.frag");

  // Iluminação no fragment shader aka Blinn Phong Shading
  sp3 = myInit_BlinnPhong(xanela, "t3.vert", "bp.frag" );

  // Blinn Phong Shading com TEXTURA
  sp4 = myInit_IlumTexture(fenestra, "t4.vert", "bpt.frag" );

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);



  do
    {
      myDisplay1(sp1);
      myDisplay2(sp2);
      myDisplay3(sp3);
      myDisplay4(sp4);

      angulo += 0.5;

      glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );


  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

