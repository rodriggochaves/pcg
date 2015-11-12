// In this version, idle function increments angles
// which are sent to vertex shader where rotation takes place

#include "Angel.h"
#include "./Ralha.h"
#include <math.h>
#include <SOIL/SOIL.h>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

/* (6 faces)(2 triangles/face)(3 vertices/triangle)  */
const int NumVertices = 36;

point4 points[NumVertices];
color4 colors[NumVertices];

// Variaveis do cubo
point4 vertices[8] = {
  point4( -0.5, -0.5,  0.5, 1.0 ),
  point4( -0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5, -0.5,  0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4( -0.5,  0.5, -0.5, 1.0 ),
  point4(  0.5,  0.5, -0.5, 1.0 ),
  point4(  0.5, -0.5, -0.5, 1.0 ),
};


// Variaveis para cor e textura
GLfloat TextureExist;
GLfloat colorr;
GLfloat colorg;
GLfloat colorb;

enum {Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3};
float escala = .125;
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat Posx = 0.0;
GLfloat Posy = -3.0;
GLfloat Angulo = 0.0;
GLfloat Posx2 = 0.0;
GLfloat Posy2 = 3.0;
GLfloat Angulo2 = 180;
GLfloat Posxbala = 0.0;
GLfloat Posybala = 0.0;
GLfloat Angulobala = 0.0;
GLfloat Posxbala2 = 0.0;
GLfloat Posybala2 = 0.0;
GLfloat Angulobala2 = 0.0;
GLfloat AnguloCano = 0.0;
GLfloat AnguloCano2 = 0.0;
GLfloat Angulomira = 0.0;
GLfloat Angulomira2 = 0.0;
int atirou = 0.0;
int atirou2 = 0.0;
int pontos = 0;
int pontos2 = 0;
int tempodetiro = 0;
int tempodetiro2 = 0;

GLfloat theta; // The location of the "theta" shader uniform variable
GLfloat velocidade = 0.0;
GLfloat velocidade2 = 0.0;
GLfloat velocidadebala = 0.0;
GLfloat velocidadebala2 = 0.0;
GLuint mvmLoc;
GLuint posicaocubo;
GLuint rotacaocubo;
mat4 ModelView;
mat4 Posicaom;
mat4 Rotacaom;

bool* keyStates = new bool[256];
//--------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//   to the vertices

int Index = 0; 

void quad( int a, int b, int c, int d )
{
 points[Index] = vertices[a]; Index++;
 points[Index] = vertices[b]; Index++;
 points[Index] = vertices[c]; Index++;
 points[Index] = vertices[a]; Index++;
 points[Index] = vertices[c]; Index++;
 points[Index] = vertices[d]; Index++;
}



//--------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );

  colors[0] = color4(1,1,1,1);
  colors[1] = color4(1,1,1,1);
  colors[2] = color4(1,1,1,1);
  colors[3] = color4(1,1,1,1);
  colors[4] = color4(1,1,1,1);
  colors[5] = color4(1,1,1,1);

  colors[6] = color4(1,1,1,1);
  colors[7] = color4(1,1,1,1);
  colors[8] = color4(0,0,0,1);
  colors[9] = color4(1,1,1,1);
  colors[10] = color4(0,0,0,1);
  colors[11] = color4(0,0,0,1);

  colors[12] = color4(1,1,1,1);
  colors[13] = color4(1,1,1,1);
  colors[14] = color4(0,0,0,1);
  colors[15] = color4(1,1,1,1);
  colors[16] = color4(0,0,0,1);
  colors[17] = color4(0,0,0,1);

  colors[18] = color4(0,0,0,1);
  colors[19] = color4(0,0,0,1);
  colors[20] = color4(1,1,1,1);
  colors[21] = color4(0,0,0,1);
  colors[22] = color4(1,1,1,1);
  colors[23] = color4(1,1,1,1);

  colors[24] = color4(0,0,0,1);
  colors[25] = color4(0,0,0,1);
  colors[26] = color4(0,0,0,1);
  colors[27] = color4(0,0,0,1);
  colors[28] = color4(0,0,0,1);
  colors[29] = color4(0,0,0,1);

  colors[30] = color4(0,0,0,1);
  colors[31] = color4(0,0,0,1);
  colors[32] = color4(1,1,1,1);
  colors[33] = color4(0,0,0,1);
  colors[34] = color4(1,1,1,1);
  colors[35] = color4(1,1,1,1);
}

//--------------------------------------------------------------------

// Variaveis da textura
GLuint texture;

vec2    tex_coords[4] = {
  vec2 (-0.5, -0.5),   vec2 (-0.5, 0.5), 
  vec2 (0.5, 0.5),   vec2 (0.5, -0.5)
};

//funcao de mira automaticamente
int aimbot(GLfloat posx, GLfloat posy, GLfloat posx2, GLfloat posy2, GLfloat Angulo , GLfloat Angulomira){
  float anguloesperado;
  float anguloentremiraetanque;
  float disx = posx - posx2;
  float disy = posy - posy2;
  float anguloentretanques = (180/3.14159265359)*atanf(disy/disx);
  float anguloatual = Angulomira + Angulo;
  while(anguloatual > 360){
    anguloatual -= 360;
  }
  while(anguloatual < 0){
    anguloatual += 360;
  }
   if (disx > 0)
     anguloesperado = 270 - anguloentretanques;
   else if (disx < 0)
     anguloesperado = 90 - anguloentretanques;
   else if (disy > 0)
     anguloesperado = 180;
   else anguloesperado = 0;
   if (anguloesperado < anguloatual)
     anguloentremiraetanque = anguloatual - anguloesperado;
   else 
      anguloentremiraetanque = anguloesperado - anguloatual;
   printf("%f %f\n", anguloatual, anguloesperado);
   printf("%f\n",anguloentremiraetanque);

   if (anguloentremiraetanque < 180 && anguloatual > anguloesperado)
      return -1;
   else if (anguloentremiraetanque > 180 && anguloatual > anguloesperado)
      return 1;
   else if (anguloentremiraetanque < 180 && anguloatual < anguloesperado)
      return 1;
  else if (anguloentremiraetanque > 180 && anguloatual < anguloesperado)
      return -1;
    else return 0;
}

//funcoes aceleracao nas duas direcoes
GLfloat recalculaposicaox(GLfloat velocidade, GLfloat angulo, GLfloat posx){
  float angulosradian;
  angulosradian = (3.14159265359/180)*angulo;
  float seno = sin(angulosradian);
  if (posx < 1/escala && seno*velocidade > 0 ){
    posx += velocidade*seno;
  }
  if (posx > -1/escala && seno*velocidade < 0 ){
    posx += velocidade*seno;
  }
  return posx;
}

GLfloat recalculaposicaoy(GLfloat velocidade, GLfloat angulo, GLfloat posy){
  float angulosradian;
  angulosradian = (3.14159265359/180)*angulo;
  float cose = cos(angulosradian);
  if (posy < 1/escala && cose*velocidade > 0 ){
    posy += velocidade*cose;
  }
  if (posy > -1/escala && cose*velocidade < 0 ){
    posy += velocidade*cose;
  }
  return posy;
}

//funcao que desacelera o tanque com o tempo
GLfloat atrito(GLfloat velocidade){
  if (velocidade > 0.0005){
    velocidade -= 0.0005;
  }
  else if (velocidade >= -0.0005){
    velocidade = 0;
  } 
  else velocidade += 0.0005;
  return velocidade;
}

//calcula se o tanque saiu do quadrado 
int saiu(GLfloat posx, GLfloat posy){
  if (posy > 1/escala || posy < -1/escala){
    return 1;
  }
  else if (posx > 1/escala || posx < -1/escala){
    return 1;
  }
  return 0;
}

//funcao que verifica se o tiro do tanque esta dentro do outro
int verificasetahdentro(GLfloat posx, GLfloat posy, GLfloat posx2, GLfloat posy2){
  float distanciax = posx - posx2;
  float distanciay = posy - posy2;
  float distanciatotal = sqrt((distanciay*distanciay)+(distanciax*distanciax));
  if (distanciatotal < 0.5){
    return 1;
  }
  else
    return 0;
}

// OpenGL initialization


void init( void ) 
{

  // Initialize texture objects
  glGenTextures( 1, &texture );

  int width, height;
  unsigned char* image =
    SOIL_load_image("./road-texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);

  glBindTexture( GL_TEXTURE_2D, texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image );

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

  SOIL_free_image_data(image);
  glActiveTexture( GL_TEXTURE0 );

  colorcube( );

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader36.glsl", "fshader36.glsl" );
  glUseProgram( program );

  // Create a vertex array object
  GLuint vao;
  glGenVertexArraysAPPLE( 1, &vao );
  glBindVertexArrayAPPLE( vao );

  // Create and initalize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(tex_coords), 
                  NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) , sizeof(tex_coords), tex_coords );

  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, 
          BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
			 BUFFER_OFFSET( sizeof(points) ) );

  GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
  glEnableVertexAttribArray( vTexCoord );
  glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
       BUFFER_OFFSET( sizeof(points) + sizeof(colors) ) );

  glUniform1i( glGetUniformLocation(program, "texture"), 0 );
  mvmLoc = glGetUniformLocation( program, "mvm" );
  rotacaocubo = glGetUniformLocation( program, "rotacaom" );
  posicaocubo = glGetUniformLocation( program, "posicaom" );
  TextureExist = glGetUniformLocation( program, "textureexist" );
  colorr = glGetUniformLocation( program, "colorR" );
  colorg = glGetUniformLocation( program, "colorG" );
  colorb = glGetUniformLocation( program, "colorB" );

  glutIgnoreKeyRepeat(true);
  glEnable( GL_DEPTH_TEST );
  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}


//-------------------------------------------------------------------------


void display( void) 
{
  
  glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
  ModelView = Scale(escala)*RotateZ(90)*RotateY(-50);
  glUniformMatrix4fv(mvmLoc, 1, GL_TRUE, ModelView);

  // chao
  glUniform1f(TextureExist, 1);

  glUniform1f(colorr, 0.5);
  glUniform1f(colorg, 0.5);
  glUniform1f(colorb, 0.5);
  Posicaom = Translate(0, 0, 0);
  Rotacaom = RotateZ(0)*Scale(17,17,1);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, 3);

  Posicaom = Translate(0, 0, 0);
  Rotacaom = RotateZ(180)*Scale(17,17,1);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, 3);

  glUniform1f(TextureExist, 0);

  //quadrado 1
  glUniform1f(colorr, 0);
  glUniform1f(colorg, 1);
  glUniform1f(colorb, 0);
  Posicaom = Translate(Posx, Posy, 0);
  Rotacaom = RotateZ(-Angulo)*Scale(0.563,0.85,0.274);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //parte de cima do quadrado 1
  Rotacaom = RotateZ(-Angulo)*Translate(0, 0.1, -0.1)*RotateZ(-Angulomira)*Scale(0.4,0.4,0.4);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //cano quadrado 1
  Rotacaom = RotateX(-AnguloCano)*RotateZ(-Angulo)*Translate(0, 0.1, -0.1)*RotateZ(-Angulomira)*Translate(0, 0.3, -0.1)*Scale(0.1,0.4,0.1);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //Bala do quadrado 1
  Posicaom = Translate(Posxbala, Posybala, 0);
  Rotacaom = RotateX(-AnguloCano)*RotateZ(-Angulo)*Translate(0, 0.1, -0.1)*RotateZ(-Angulomira)*Translate(0, 0.3, -0.1)*Scale(0.1,0.4,0.1);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //quadrado 2
  glUniform1f(colorr, 1);
  glUniform1f(colorg, 1);
  glUniform1f(colorb, 0);
  Posicaom = Translate(Posx2, Posy2, 0);
  Rotacaom = RotateZ(-Angulo2)*Scale(0.563,0.85,0.274);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //parte de cima do quadrado 2
  Rotacaom = RotateZ(-Angulo2)*Translate(0, 0.1, -0.1)*RotateZ(-Angulomira2)*Scale(0.4,0.4,0.4);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //cano quadrado 2
  Rotacaom = RotateX(-AnguloCano2)*RotateZ(-Angulo2)*Translate(0, 0.1, -0.1)*RotateZ(-Angulomira2)*Translate(0, 0.3, -0.1)*Scale(0.1,0.4,0.1);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  //Bala do quadrado 2
  Posicaom = Translate(Posxbala2, Posybala2, 0);
  Rotacaom = RotateX(-AnguloCano2)*RotateZ(-Angulo2)*Translate(0, 0.1, -0.1)*RotateZ(-Angulomira2)*Translate(0, 0.3, -0.1)*Scale(0.1,0.4,0.1);
  glUniformMatrix4fv(posicaocubo, 1, GL_TRUE, Posicaom);
  glUniformMatrix4fv(rotacaocubo, 1, GL_TRUE, Rotacaom);
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  glutSwapBuffers( );


}


void keyOperations(){
  if (velocidade < 0.1){
    if (keyStates['w']){
      velocidade += 0.001;
    }
  }
  if (velocidade > -0.1){
    if (keyStates['s']){
      velocidade -= 0.001;
    }
  }
  if (keyStates['a']){
    Angulo -= 0.5;
  }
  if (keyStates['d']){
    Angulo += 0.5;
  }
  if (velocidade2 < 0.1){
    if (keyStates['u']){
      velocidade2 += 0.001;
    }
  }
  if (velocidade2 > -0.1){
    if (keyStates['j']){
      velocidade2 -= 0.001;
    }
  }
  if (keyStates['h']){
    Angulo2 -= 0.5;
  }
  if (keyStates['k']){
    Angulo2 += 0.5;
  }
  if (keyStates['r']){
    Angulomira -= 0.5;
  }
  if (keyStates['t']){
    Angulomira += 0.5;
  }
  if (keyStates['o']){
    Angulomira2 -= 0.5;
  }
  if (keyStates['p']){
    Angulomira2 += 0.5;
  }
  if (keyStates['e']){
    Angulomira += aimbot(Posx, Posy, Posx2, Posy2, Angulo , Angulomira);
  }
}

void keyPressed (unsigned char key, int x, int y) {  
keyStates[key] = true; // Set the state of the current key to pressed  
  if (key == 'f' && tempodetiro == 0){
    atirou = 1;
    tempodetiro = 100;
  }
  if (key == 'l' && tempodetiro2 == 0){
    atirou2 = 1;
    tempodetiro2 = 100;
  }
}  
  
void keyUp (unsigned char key, int x, int y) {  
keyStates[key] = false; // Set the state of the current key to not pressed  
}  

//-------------------------------------------------------------------------
void idle( void )
{
  Posy = recalculaposicaoy(velocidade,Angulo,Posy);
  Posx = recalculaposicaox(velocidade,Angulo,Posx);
  velocidade = atrito(velocidade);
  Posy2 = recalculaposicaoy(velocidade2,Angulo2,Posy2);
  Posx2 = recalculaposicaox(velocidade2,Angulo2,Posx2);
  velocidade2 = atrito(velocidade2);
  keyOperations();
  if (tempodetiro > 0){
    tempodetiro -= 1;
  }
  if (tempodetiro2 > 0){
    tempodetiro2 -= 1;
  }
  if (atirou){
    Posybala = recalculaposicaoy(0.5,Angulobala,Posybala);
    Posxbala = recalculaposicaox(0.5,Angulobala,Posxbala);
    if (verificasetahdentro(Posxbala,Posybala,Posx2,Posy2)){
      atirou = 0;
      pontos += 100;
      printf("Pontos jogador 1 : %d\nPontos jogador 2 : %d\n",pontos,pontos2);
    }
    if (saiu(Posxbala,Posybala)){
      atirou = 0;
    }
  }
  else{
    Posxbala = Posx;
    Posybala = Posy;
    velocidadebala = velocidade;
    Angulobala = Angulo+Angulomira;
  }

  if (atirou2){
    Posybala2 = recalculaposicaoy(0.5,Angulobala2,Posybala2);
    Posxbala2 = recalculaposicaox(0.5,Angulobala2,Posxbala2);
    if (verificasetahdentro(Posxbala2,Posybala2,Posx,Posy)){
      atirou2 = 0;
      pontos2 += 100;
      printf("Pontos jogador 1 : %d\nPontos jogador 2 : %d\n",pontos,pontos2);
    }
    if (saiu(Posxbala2,Posybala2)){
      atirou2 = 0;
    }
  }
  else{
    Posxbala2 = Posx2;
    Posybala2 = Posy2;
    velocidadebala2 = velocidade2;
    Angulobala2 = Angulo2+Angulomira2;
  }
  glutPostRedisplay( );
}

//-------------------------------------------------------------------------

int main (int argc, char **argv) 
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 800, 800 );
  glutCreateWindow( "Joguinho de tanques" );
  inicia_glew();
  // glewInit( );
  init( );
  glutDisplayFunc( display );
  glutKeyboardFunc( keyPressed );
  glutKeyboardUpFunc( keyUp );
  glutIdleFunc( idle );

  glutMainLoop( );

  return 0;
}
