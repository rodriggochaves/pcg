// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM - math
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

// custom includes learnopengl -includes do tutorial
#include "learnopenglincludes/shader.h"
#include "learnopenglincludes/camera.h"
#include "learnopenglincludes/filesystem.h"
#include "learnopenglincludes/model.h"

// custom assimp includes
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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

// Properties
GLuint screenWidth = 800, screenHeight = 600;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vextershader.vert", 
        "fragmentshader.frag" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // Setup and compile our shaders
    Shader shader("cubemaps.vs", "cubemaps.frag");
    Shader skyboxShader("skybox.vs", "skybox.frag");

#pragma region "object_initialization"
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

    
    // Setup skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

#pragma endregion

    // Cubemap (Skybox)
    std::vector<const GLchar*> faces;
    faces.push_back("right.jpg");
    faces.push_back("left.jpg");
    faces.push_back("top.jpg");
    faces.push_back("bottom.jpg");
    faces.push_back("back.jpg");
    faces.push_back("front.jpg");
    //carrega na skybox as imagens
    GLuint skyboxTexture = loadCubemap(faces);

    // Projection matrix : 45° Field of View, 4:3 ratio, 
    // display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 
        4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
                                // Camera is at (4,3,3), in World Space
                                glm::vec3(4,3,3),
                                // and looks at the origin
                                glm::vec3(0,0,0),
                                // Head is up (set to 0,-1,0 
                                // to look upside-down) 
                                glm::vec3(0,1,0) 
                           );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model; // Remember, 
    // matrix multiplication is the other way around

    // Load the texture using any two methods
    GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    // GLuint Texture = loadDDS("uvtemplate.DDS");
    
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Our vertices. Tree consecutive floats give a 3D vertex; 
    // Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each,
    //  so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
    };

    // Two UV coordinatesfor each vertex. They were created withe Blender.
    static const GLfloat g_uv_buffer_data[] = { 
        0.000059f, 1.0f-0.000004f, 
        0.000103f, 1.0f-0.336048f, 
        0.335973f, 1.0f-0.335903f, 
        1.000023f, 1.0f-0.000013f, 
        0.667979f, 1.0f-0.335851f, 
        0.999958f, 1.0f-0.336064f, 
        0.667979f, 1.0f-0.335851f, 
        0.336024f, 1.0f-0.671877f, 
        0.667969f, 1.0f-0.671889f, 
        1.000023f, 1.0f-0.000013f, 
        0.668104f, 1.0f-0.000013f, 
        0.667979f, 1.0f-0.335851f, 
        0.000059f, 1.0f-0.000004f, 
        0.335973f, 1.0f-0.335903f, 
        0.336098f, 1.0f-0.000071f, 
        0.667979f, 1.0f-0.335851f, 
        0.335973f, 1.0f-0.335903f, 
        0.336024f, 1.0f-0.671877f, 
        1.000004f, 1.0f-0.671847f, 
        0.999958f, 1.0f-0.336064f, 
        0.667979f, 1.0f-0.335851f, 
        0.668104f, 1.0f-0.000013f, 
        0.335973f, 1.0f-0.335903f, 
        0.667979f, 1.0f-0.335851f, 
        0.335973f, 1.0f-0.335903f, 
        0.668104f, 1.0f-0.000013f, 
        0.336098f, 1.0f-0.000071f, 
        0.000103f, 1.0f-0.336048f, 
        0.000004f, 1.0f-0.671870f, 
        0.336024f, 1.0f-0.671877f, 
        0.000103f, 1.0f-0.336048f, 
        0.336024f, 1.0f-0.671877f, 
        0.335973f, 1.0f-0.335903f, 
        0.667969f, 1.0f-0.671889f, 
        1.000004f, 1.0f-0.671847f, 
        0.667979f, 1.0f-0.335851f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), 
        g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data),
        g_uv_buffer_data, GL_STATIC_DRAW);

    // Setup and compile our shaders
    Shader unicornShader("unicornShader.vs", "unicornShader.frag");

    //carregando o modelo da bola
    Model::Model unicornModel("planet/planet.obj");

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

        // Cubes
        //desenha a skybox
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        
        //desenha a bola
        unicornShader.Use();   // <-- Don't forget this one!
        glUniformMatrix4fv(glGetUniformLocation(unicornShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(unicornShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        //mudando a posição da bola e o tamanho dela
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // It's a bit too big for our scene, so scale it down
        // model = glm::rotate(model, (glm::mediump_float)45.0, glm::vec3(1, 1, 1));
        glUniformMatrix4fv(glGetUniformLocation(unicornShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        unicornModel.Draw(unicornShader);
        
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
        glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default

        // Swap buffers
        glfwSwapBuffers(window);


    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
    //limpando a casa
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