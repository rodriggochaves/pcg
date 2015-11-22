#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

//#include <glm/glm.hpp>
#include "../includes/Ralha.h"

#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide :
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

// - Adaptação feita por Ralha em setembro de 2015

bool loadOBJ(
	const char * path,
	std::vector<Angel::vec3> & out_vertices,
	std::vector<Angel::vec2> & out_uvs,
	std::vector<Angel::vec3> & out_normals
){
  printf("\nCódigo original disponível em ");
  printf("http://www.opengl-tutorial.org/beginners-tutorials/");
  printf("tutorial-7-model-loading/\n");

  printf("Loading OBJ file %s...\n", path);

  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;;
  std::vector<Angel::vec3> temp_vertices;
  std::vector<Angel::vec2> temp_uvs;
  std::vector<Angel::vec3> temp_normals;


  FILE * file = fopen(path, "r");
  if( file == NULL )
    {
      printf("Impossible to open the file ! Are you in the right path?\n");
      getchar();
      return false;
    }

  while( 1 )
    {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF)
	break; // EOF = End Of File. Quit the loop.
      // else : parse lineHeader

      if ( strcmp( lineHeader, "v" ) == 0 )
	{
	  Angel::vec3 vertex;
	  fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
	  temp_vertices.push_back(vertex);
	}
      else if ( strcmp( lineHeader, "vt" ) == 0 )
	{
	  Angel::vec2 uv;
	  fscanf(file, "%f %f\n", &uv.x, &uv.y );
	  uv.y = -uv.y; // Invert V coord since we'll only use DDS texture,
	  // which are inverted. Remove if you want to use
	  // TGA or BMP loaders.
	  temp_uvs.push_back(uv);
	}
      else if ( strcmp( lineHeader, "vn" ) == 0 )
	{
	  Angel::vec3 normal;
	  fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
	  temp_normals.push_back(normal);
	}
      else if ( strcmp( lineHeader, "f" ) == 0 )
	{
	  std::string vertex1, vertex2, vertex3;
	  unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	  int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
			       &vertexIndex[0], &uvIndex[0], &normalIndex[0],
			       &vertexIndex[1], &uvIndex[1], &normalIndex[1],
			       &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
	  if (matches != 9)
	    {
	      printf("File can't be read by our simple parser :-(");
	      printf(" Try exporting with other options\n");
	      return false;
	    }

	  vertexIndices.push_back(vertexIndex[0]);
	  vertexIndices.push_back(vertexIndex[1]);
	  vertexIndices.push_back(vertexIndex[2]);
	  uvIndices    .push_back(uvIndex[0]);
	  uvIndices    .push_back(uvIndex[1]);
	  uvIndices    .push_back(uvIndex[2]);
	  normalIndices.push_back(normalIndex[0]);
	  normalIndices.push_back(normalIndex[1]);
	  normalIndices.push_back(normalIndex[2]);
	}
      else
	{
	  // Probably a comment, eat up the rest of the line
	  char stupidBuffer[1000];
	  fgets(stupidBuffer, 1000, file);
	}

    }//fim do while (1)

  // For each vertex of each triangle
  for( unsigned int i=0; i<vertexIndices.size(); i++ )
    {
      // Get the indices of its attributes
      unsigned int vertexIndex = vertexIndices[i];
      unsigned int uvIndex = uvIndices[i];
      unsigned int normalIndex = normalIndices[i];

      // Get the attributes thanks to the index
      Angel::vec3 vertex = temp_vertices[ vertexIndex-1 ];
      Angel::vec2 uv = temp_uvs[ uvIndex-1 ];
      Angel::vec3 normal = temp_normals[ normalIndex-1 ];

      // Put the attributes in buffers
      out_vertices.push_back(vertex);
      out_uvs.push_back(uv);
      out_normals.push_back(normal);

    }

  return true;
}

