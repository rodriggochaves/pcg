#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "../includes/Ralha.h"

bool loadOBJ(
	const char * path,
	std::vector<Angel::vec3> & out_vertices,
	std::vector<Angel::vec2> & out_uvs,
	std::vector<Angel::vec3> & out_normals
);




#endif
