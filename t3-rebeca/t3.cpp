#include <vector>
#include "includes/Ralha.h"
#include <SOIL/SOIL.h>
#include "common/objloader.hpp"

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

void myDisplay( void );
void myInit( void );
GLuint loadCubemap(std::vector<const GLchar*> faces);


point4 points[3] = {
  point4( -0.5, -0.5, 0.0, 1.0 ),
  point4(  0.5, -0.5, 0.0, 1.0 ),
  point4(  0.0,  0.5, 0.0, 1.0 )
};

GLfloat skyboxVertices[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


GLFWwindow *window;

GLuint programID;
GLuint Texture, TextureID;
GLuint MatrixID, VertexArrayID;
GLuint uvbuffer, vertexbuffer;
GLuint skyboxShader;
GLuint skyboxVAO;

GLfloat angulo = 0.0f;

std::vector<vec3> vertices;
std::vector<vec2> uvs;
std::vector<vec3> normals; // Não são usadas neste programa

mat4 ProjectionMatrix, ViewMatrix, ModelMatrix, MVP;

int main (int argc, char** argv) {
  	
  	window = inicia_glfw(3, 3, "Unicornio");
  	
  	inicia_glew ( );
	
	myInit();

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
	do{
		myDisplay();
		angulo += 0.5;
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );

	/* Before  your application  exits,  you should  terminate the  GLFW
	 library if it has been initialized. */
	glfwTerminate();
}


void myDisplay( void ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ModelMatrix = Ortho(-3,3,-3,3,-3,3);
	ModelMatrix =  Scale(3.50) * RotateX(100.0) * RotateY(angulo) * ModelMatrix;
	ViewMatrix = Translate(0, 0, 0);
	ProjectionMatrix = Scale(1.0);

	MVP = ProjectionMatrix * ViewMatrix  * ModelMatrix;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, MVP);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	glfwSwapBuffers(window);
}


void myInit( void ) {
	// Enable depth test
  	glEnable(GL_DEPTH_TEST);
  	// Accept fragment if it closer to the camera than the former one
  	glDepthFunc(GL_LESS);
  	GLuint vertexbuffer;

  	// Cull triangles which normal is not towards the camera
  	glEnable(GL_CULL_FACE);

  	glGenVertexArrays(1, &VertexArrayID);
  	glBindVertexArray(VertexArrayID);

	// Load shaders and use the resulting shader program
	programID = InitShader( "./tri.vert", "./tri.frag" );
	glUseProgram( programID );

  	// Create a vertex array object
  	GLuint vao;
  	glGenVertexArrays( 1, &vao );
  	glBindVertexArray( vao );

  	// manipulando as texturas
	glGenTextures( 1, &Texture );
	int width, height;
	unsigned char* imagem =
		SOIL_load_image("./Sculpt.png", &width, &height, 0, SOIL_LOAD_RGB);
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
  	bool res = loadOBJ("Sculpt.obj", vertices, uvs, normals);

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

	// an white background, plis
  	glClearColor( 1.0, 1.0, 1.0, 1.0 );
  	 
 //  	std::vector<const GLchar*> faces;
	// faces.push_back("siege_rt.tga");
	// faces.push_back("siege_lf.tga");
	// faces.push_back("siege_up.tga");
	// faces.push_back("siege_dn.tga");
	// faces.push_back("siege_bk.tga");
	// faces.push_back("siege_ft.tga");
	// GLuint cubemapTexture = loadCubemap(faces);

	// skyboxShader = InitShader( "./skybox.vert", "./skybox.frag" );

	// glDepthMask(GL_FALSE);
	// // skyboxShader.Use();
	// glUseProgram(skyboxShader);
	// // ... Set view and projection matrix
	// glEnableVertexAttribArray(2);
	// glBindVertexArray(skyboxVAO);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	// glBindVertexArray(0);
	// glDepthMask(GL_TRUE);
	// ... Draw rest of the scene 


}

GLuint loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;
	
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}  


