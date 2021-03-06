// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// Include SOIL
#include <SOIL/SOIL.h>

// custom includes common
#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"

// custom includes learnopengl
#include "learnopenglincludes/shader.h"
#include "learnopenglincludes/camera.h"
#include "learnopenglincludes/filesystem.h"
#include "learnopenglincludes/mesh.h"
#include "learnopenglincludes/model.h"

// assimp includes
#include <assimp/types.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Function prototypes
//movimentação pelas teclas
void key_callback(GLFWwindow* window, int key, int scancode, 
    int action, int mode);
//movimentação pelo scroll do mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//movimentação pelo mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//carrega o movimentação feita e carrega no loop de eventos
void Do_Movement();
//textura do planeta
GLuint loadTexture(GLchar const * path);
//skybox
GLuint loadCubemap(std::vector<const GLchar*> faces);

// Camera - visualização do 'usuario'
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//teclado
bool keys[1024];
//posição de inicio do programa
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool spinnig = false;

// Properties
GLuint screenWidth = 800, screenHeight = 600;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// PI
const GLfloat PI = 3.14159265;

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }
    //versões
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS
    // happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Trabalho Final", NULL, NULL);
    if( window == NULL ){
        printf("Failed to open GLFW window.");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

     // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

     // Set the mouse at the center of the screen
    //glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // White background
    // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Setup and compile our shaders
    Shader shader("cubemaps.vs", "cubemaps.frag");
    Shader skyboxShader("skybox.vs", "skybox.frag");

#pragma region "object_initialization"
    GLfloat cubeVertices[] = {
        // Positions          // Normals
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    // Set the object data (buffers, vertex attributes)
    GLfloat skyboxVertices[] = {
        // Positions          
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    // setup cubeVAO and cubeVBO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, 
    	GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 
    	(GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 
    	(GLvoid*)(3 * sizeof(GLfloat)));

    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, 
    	GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 
    	(GLvoid*)0);
    glBindVertexArray(0);

#pragma endregion

    // Cubemap (Skybox)
    std::vector<const GLchar*> faces;
    faces.push_back("resources/jajlands1/right.jpg");
    faces.push_back("resources/jajlands1/left.jpg");
    faces.push_back("resources/jajlands1/top.jpg");
    faces.push_back("resources/jajlands1/bottom.jpg");
    faces.push_back("resources/jajlands1/back.jpg");
    faces.push_back("resources/jajlands1/front.jpg");
    //carrega na skybox as imagens
    GLuint skyboxTexture = loadCubemap(faces);

    // Setup and compile our shaders
    Shader objShader("objshader.vs", "objshader.frag");
    // loading rock model
    Model::Model objModel("resources/rock/rock.obj");

	//nanosuit model
	Shader nanoShader("nanoshader.vs", "nanoshader.frag");
    Model::Model nanoModel("resources/nanosuit/nanosuit.obj");

    GLfloat rotAngle = 0.0f;

    do{

        // Set frame time - 'atulização da tela'
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw scene as normal
        //utilizando o shader da skybox
        shader.Use();      
        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, 
             (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 
             1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, 
             GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 
             1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(shader.Program, "cameraPos"), 
             camera.Position.x, camera.Position.y, camera.Position.z);

        // glBindVertexArray(cubeVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        objShader.Use();   // <-- Don't forget this one!
        glUniformMatrix4fv(glGetUniformLocation(objShader.Program, 
	    	"projection"), 1, GL_FALSE, glm::value_ptr(projection));
	    glUniformMatrix4fv(glGetUniformLocation(objShader.Program, "view"), 
	    	1, GL_FALSE, glm::value_ptr(view));
	   	// It's a bit too big for our scene, so scale it down
	    model = glm::translate(model, glm::vec3(-25.0f, -14.0f, -25.0f));
	    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	    if (spinnig) 
	    	model = glm::rotate(model, rotAngle, 
	    		glm::vec3(0.04f, 0.06f, 0.08f));
	    
	    // Rotation: add random rotation around a (semi)randomly picked
        
      	GLfloat X = 3;
      	GLfloat DISTANCE = 50;

        // for (int i = 0; i < X; ++i)
        // {
		    for (int j = 0; j < X; ++j)
		    {
		    	for (int k = 0; k < X; ++k)
		    	{
		    		// Translate it down a bit so 
		    		// it's at the center of the scene
				    model = glm::translate(model, glm::vec3(DISTANCE, 
				    	0.0f, 0.0f)); 
				    // rotation axis vector
				    glUniformMatrix4fv(glGetUniformLocation(objShader.Program, 
				    	"model"), 1, GL_FALSE, glm::value_ptr(model));
				    objModel.Draw(objShader);
		    	}
		    	model = glm::translate(model, glm::vec3(-DISTANCE * X, 
		    		0.0f, DISTANCE));
		    }
		    // model = glm::translate(model, glm::vec3(0.0f, -DISTANCE * X, 
		    // 	DISTANCE));  
      //   }

		// draws and moviment nanosuit
		// defines nanoMatrixs
		nanoShader.Use();
		glm::mat4 nanomodel;
		// puts the nanosuit into middle of screen
		nanomodel = glm::translate(nanomodel, glm::vec3(0.0f, -7.0f, -5.0f));
		nanomodel = glm::translate(nanomodel, camera.Position);
		
		// init the nano suit looking forward
		nanomodel = glm::rotate(nanomodel, PI,
			glm::vec3(0.0f, 1.0f, 0.0f));
		// nanomodel = glm::rotate(nanomodel, PI,
		// 	camera.Position);
		nanomodel = glm::scale(nanomodel, glm::vec3(0.5f, 0.5f, 0.5f));
        glm::mat4 nanoview = camera.GetViewMatrix();
        glm::mat4 nanoprojection = glm::perspective(camera.Zoom, 
             (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

        //fits the nanoMatrixs to the shaders
        glUniformMatrix4fv(glGetUniformLocation(nanoShader.Program, "model"), 
             1, GL_FALSE, glm::value_ptr(nanomodel));
        glUniformMatrix4fv(glGetUniformLocation(nanoShader.Program, "view"), 1, 
             GL_FALSE, glm::value_ptr(nanoview));
        glUniformMatrix4fv(glGetUniformLocation(nanoShader.Program, 
        	"projection"), 
             1, GL_FALSE, glm::value_ptr(nanoprojection));
        nanoModel.Draw(nanoShader);

        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth 
        // test passes when values are equal to depth buffer's content
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    
        // Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, 
            "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default

        // Swap buffers
        glfwSwapBuffers(window);

        rotAngle += 0.1f;

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    
    // housekeeping
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width, height;
    unsigned char* image;
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    //carregando as imagens nos lados do cubo
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, 
            height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}


// This function loads a texture from file. Note: texture loading 
// functions like these are usually 
// managed by a 'Resource Manager' that manages all resources 
// (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
// GLuint loadTexture(GLchar const * path)
// {
//     //Generate texture ID and load texture data 
//     GLuint textureID;
//     glGenTextures(1, &textureID);
//     int width, height;
//     unsigned char* image = SOIL_load_image(path, &width, &height, 0, 
//         SOIL_LOAD_RGB);
//     // Assign texture to ID
//     glBindTexture(GL_TEXTURE_2D, textureID);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, 
//         GL_UNSIGNED_BYTE, image);
//     glGenerateMipmap(GL_TEXTURE_2D);

//     // Parameters
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
//         GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glBindTexture(GL_TEXTURE_2D, 0);
//     SOIL_free_image_data(image);
//     return textureID;
// }

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (keys[GLFW_KEY_F])
    	spinnig = !spinnig;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode,
     int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

#pragma endregion