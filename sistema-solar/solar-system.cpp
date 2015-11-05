#include <vector>
/* Ordem das inclusões é importante!
   A classe C++ vector é usada em objloader.cpp
   Por isso, a inclusão de vector tem que preceder a inclusão de objloader.h
*/
#include "../includes/Ralha.h"
#include <SOIL/SOIL.h>


#include "../common/objloader.hpp"


GLFWwindow* window;

using namespace std; //não precisaremos escrever std::vector<vec3> vertices;

GLuint programID;
GLuint Texture, TextureID;
GLuint MatrixID, VertexArrayID;
GLuint uvbuffer, vertexbuffer;

GLfloat angulo = 0.0f;

vector<vec3> vertices;
vector<vec2> uvs;
vector<vec3> normals; // Não são usadas neste programa

mat4 ProjectionMatrix, ViewMatrix, ModelMatrix, MVP;

void myInit ( void )
{
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetCursorPos(window, 800/2, 800/2);

  // Space is supposed to be black
  glClearColor(.0f, .0f, .0f, 1.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  GLuint vertexbuffer;

  // Cull triangles which normal is not towards the camera
  glEnable(GL_CULL_FACE);

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  programID = InitShader( "transf.vert", "transf.frag" );

  // manipulando as texturas
  glGenTextures( 1, &Texture );
  int width, height;
  unsigned char* imagem =
    SOIL_load_image("./planet_Quom1200.png", &width, &height, 0, SOIL_LOAD_RGB);
  glBindTexture( GL_TEXTURE_2D, Texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, imagem );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

  SOIL_free_image_data(imagem);


  // Get a handle for our "myTextureSampler" uniform
  TextureID  = glGetUniformLocation(programID, "myTextureSampler");

  // Read our .obj file
  bool res = loadOBJ("planet.obj", vertices, uvs, normals);

  // Load it into a VBO
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3),
	       &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2),
	       &uvs[0], GL_STATIC_DRAW);

  glUseProgram(programID);

  // Bind our texture in Texture Unit 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  glUniform1i(TextureID, 0);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer( 1,	2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
}

/*
  As coordenadas do planet.obj cabem num cubo (-3, -3, -3) a
  (3, 3, 3)
 */
void myDisplay (void )
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // 1º colocamos a esfera dentro do Cubo de Modelagem
  // Como a maior coordenada é em valor absoluto < 4.0
  // usaremos a matrix Orto para trazer a esfera
  // pra dentro do cubo de modelagem
  ModelMatrix = Ortho(-3,3,-3,3,-3,3);
  // Agora vamos Rodar a esfera em Y e encolhe-la em 80%
  ModelMatrix =  Scale(.15) * RotateY(1.5*angulo) * ModelMatrix;
  // ModelMatrix = Translate(2, 0, 0);// * Scale(.15) * RotateY(1.5*angulo) * ModelMatrix;
  // ModelMatrix = Scale(.20) * RotateZ(1.5*angulo) * RotateY(1.5*angulo) * ModelMatrix;
  // ModelMatrix = Scale(.20) * RotateY(1.5*angulo) * Translate(0, 0, 2);
  // Como vamos usar a projeção ortogonal não mexemos na
  // matriz de visualização
  ViewMatrix = Translate(2, 0, 0);
  // ViewMatrix = Scale(1.0) * Translate(0, 0, -2);
  // E como já estamos dentro do cubo canônico não bulimos com
  // a matriz de projeção
  ProjectionMatrix = Scale(1.0);
  // ProjectionMatrix = Ortho(-5,5,-5,5,-5,5);

  MVP = ProjectionMatrix * ViewMatrix  * ModelMatrix;

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, MVP);
  // Desenhe o exoplaneta
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
  // Swap buffers
  glfwSwapBuffers(window);
}



int main( void )
{
  window = inicia_glfw(3, 3, "Sistema Solar");
  if( window == NULL )
    {
      fprintf( stderr, "Failed to open GLFW window.\n");
      glfwTerminate();
      return -1;
    }

  inicia_glew();

  myInit();


  do
    {
      myDisplay();
      angulo += 0.5;
      glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );

  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteProgram(programID);
  glDeleteTextures(1, &TextureID);
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

