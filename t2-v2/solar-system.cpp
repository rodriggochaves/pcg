/*
Rebeca Andrade Baldomir

Principios de Programação Gráfica - Turma A
Sistema Solar
 */


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


GLFWwindow *window;

using namespace std; //não precisaremos escrever std::vector<vec3> vertices;

GLuint program;
GLuint VAO;

GLuint vb, uvb, nb;

GLfloat angulo = 0.0f;

vector<vec3> vertices, normals;
vector<vec2> uvs;

mat4 projection, view, model;
GLuint projectionLoc, viewLoc, modelLoc;
GLuint textureLoc, texture;



/******************************************************/
/*    Janela: Esfera com textura com ilumação         */
/******************************************************/
GLuint myInit_IlumTexture(GLFWwindow* fenetre, const char* vs, const char* fs)
{
  glfwMakeContextCurrent(fenetre);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( vs, fs );
  glUseProgram(program);

  // define a cor de fundo da janela: fundo ciano
  // glClearColor(0.0, 1.0, 1.0, 1.0f);
  glClearColor(0.9, 0.9, 0.9, 0.9f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // Read our .obj file
  bool res = loadOBJ("esfera.obj", vertices, uvs, normals);

  // precisamos carregar as coordenadas de textura na GPU
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3),
	       &vertices[0], GL_STATIC_DRAW);
  // precisamos carregar as coordenadas de textura na GPU
  glGenBuffers(1, &nb);
  glBindBuffer(GL_ARRAY_BUFFER, nb);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3),
	       &normals[0], GL_STATIC_DRAW);
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
    SOIL_load_image("./preview_sun.jpg", &width, &height, 0, SOIL_LOAD_RGB);
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
  color4 light_ambient(  0.3, 0.3, 0.3, 1.0 );
  color4 light_diffuse(  0.3, 0.3, 0.3, 1.0 );
  color4 light_specular( 1.0, 1.0, 1.0, 1.0 );

  color4 material_ambient(  1.0, 0.8, 1.0, 1.0 );
  color4 material_diffuse(  1.0, 0.8, 0.8, 1.0 );
  color4 material_specular( 1.0, 1.0, 1.0, 1.0 );
  float  material_shininess = 250.0;

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

/******************************************************/
/*    Janela: Esfera com textura com ilumação         */
/******************************************************/
GLuint myInit_Sun(GLFWwindow* fenetre, const char* vs, const char* fs)
{
  glfwMakeContextCurrent(fenetre);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( vs, fs );
  glUseProgram(program);

  // define a cor de fundo da janela: fundo ciano
  // glClearColor(0.0, 1.0, 1.0, 1.0f);
  glClearColor(0.9, 0.9, 0.9, 0.9f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // Read our .obj file
  bool res = loadOBJ("esfera.obj", vertices, uvs, normals);

  // precisamos carregar as coordenadas de textura na GPU
  glGenBuffers(1, &vb);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3),
	       &vertices[0], GL_STATIC_DRAW);
  // precisamos carregar as coordenadas de textura na GPU
  glGenBuffers(1, &nb);
  glBindBuffer(GL_ARRAY_BUFFER, nb);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3),
	       &normals[0], GL_STATIC_DRAW);
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
    SOIL_load_image("./preview_sun.jpg", &width, &height, 0, SOIL_LOAD_RGB);
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
  color4 light_ambient(  0.3, 0.3, 0.3, 1.0 );
  color4 light_diffuse(  1.0, 1.0, 1.0, 1.0 );
  color4 light_specular( 10000.0, 10000.0, 10000.0, 1.0 );

  color4 material_ambient(  1.0, 0.8, 1.0, 1.0 );
  color4 material_diffuse(  1.0, 0.8, 0.8, 1.0 );
  color4 material_specular( 1.0, 1.0, 1.0, 1.0 );
  float  material_shininess = 250.0;

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

/******************************************************/
/*******************MY DISPLAY FOR SUN*****************/
/******************************************************/

void myDisplaySun ( GLuint sp4 )
{
  glfwMakeContextCurrent(window);

  // Clear the screen
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram( sp4 );

  // Retrieve transformation uniform variable locations ( sp3 )
  modelLoc  = glGetUniformLocation( sp4, "Model" );
  viewLoc = glGetUniformLocation( sp4, "View" );
  projectionLoc = glGetUniformLocation( sp4, "Projection" );

  model = Scale(2.0) * RotateY(-1.5*angulo);
  // view = Translate(0, 0, -20);
  view = Translate(0, 0, 0);
  projection = Ortho(-15,15,-15,15,-15,15);
  // projection = Perspective(30, 1, 0.01, 100.0);
  // Passando as matrizes para o vertex shader em sp1
  glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
  glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
  // Notem que view é na verdade a modelview
  glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );


  // Desenhe a esfera
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  // glfwSwapBuffers(window);

  glUseProgram( 0 );
}


/******************************************************/
/*******************MY DISPLAY FOR PLANET**************/
/******************************************************/

void myDisplayPlanet ( GLuint sp4 )
{
  glfwMakeContextCurrent(window);

  // Clear the screen
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram( sp4 );

  // Retrieve transformation uniform variable locations ( sp3 )
  modelLoc  = glGetUniformLocation( sp4, "Model" );
  viewLoc = glGetUniformLocation( sp4, "View" );
  projectionLoc = glGetUniformLocation( sp4, "Projection" );

  model = Scale(.50) * RotateY(-1.5*angulo);
  // view = Translate(0, 0, -20);
  view = RotateY(2 * angulo) * RotateX(0.5 * angulo) * Translate(12, 3, 0);
  projection = Ortho(-15,15,-15,15,-15,15);
  // projection = Perspective(30, 1, 0.01, 100.0);
  // Passando as matrizes para o vertex shader em sp1
  glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
  glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
  // Notem que view é na verdade a modelview
  glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );


  // Desenhe a esfera
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  // glfwSwapBuffers(window);

  glUseProgram( 0 );
}

/******************************************************/
/*******************MY DISPLAY FOR PLANET**************/
/******************************************************/

void myDisplayMoon ( GLuint sp4)
{
	glfwMakeContextCurrent(window);

	glUseProgram( sp4 );

	// Retrieve transformation uniform variable locations ( sp3 )
	modelLoc  = glGetUniformLocation( sp4, "Model" );
	viewLoc = glGetUniformLocation( sp4, "View" );
	projectionLoc = glGetUniformLocation( sp4, "Projection" );

	model = Scale(.25) * RotateY(-1.5 * angulo);
	// view = Translate(0, 0, -20);
	view = RotateY(2 * angulo) * RotateX(0.5 * angulo) * Translate(12, 3, 0) * RotateY(3 * angulo) * RotateX(0.5 * angulo) * Translate(3, 1, 0);
	projection = Ortho(-16,16,-16,16,-16,16);
	// Passando as matrizes para o vertex shader em sp4
	glUniformMatrix4fv( modelLoc, 1, GL_TRUE, model );
	glUniformMatrix4fv( viewLoc, 1, GL_TRUE, view );
	// Notem que view é na verdade a modelview
	glUniformMatrix4fv( projectionLoc, 1, GL_TRUE, projection );

	// Desenhe a esfera
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

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

	window =  glfwCreateWindow( 800, 800, "Sistema Solar", NULL, NULL);

	if( window == NULL ){
		fprintf( stderr, "Falha na abertura de janela GLFW.\n" );
		fprintf( stderr, "\nO programa será encerrado\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}


	glfwMakeContextCurrent(window);
	inicia_glew();

	GLuint planet;//nomes dos Shaders Programs
	planet = myInit_IlumTexture(window, "t4.vert", "bpt.frag" );

	GLuint sun;//nomes dos Shaders Programs
	sun = myInit_Sun(window, "t4.vert", "bpt.frag" );

	GLuint moon;//nomes dos Shaders Programs
	moon = myInit_IlumTexture(window, "t4.vert", "bpt.frag" );

	// set up to exit with esc key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  	do
   	{
   		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
		myDisplaySun(sun);
  		myDisplayPlanet(planet);
  		myDisplayMoon(moon);

		angulo += 0.5;

		glfwPollEvents();
		glfwSwapBuffers(window);

    } // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );


  	// Close OpenGL window and terminate GLFW
  	glfwTerminate();

  	return 0;
}

