#ifndef _CUBE_H
#define	_CUBE_H

#include <windows.h>
#include <GL/gl.h>    /* needed for all OpenGL functions & values */

class Cube
{

public:
	Cube(void);
	~Cube(void);
	void draw_cube(void);

	static float cube_vertices[9][3];
	static int cube_polygons[6][4];
	static float cube_normals[6][3];

};

#endif