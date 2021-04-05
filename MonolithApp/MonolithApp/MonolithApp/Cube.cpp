#include "Cube.h"


Cube::Cube(void)
{
}


Cube::~Cube(void)
{
}


void Cube::draw_cube(void)
{
    int poly;

    for (poly = 0; poly < 6; poly++) {
        glBegin(GL_POLYGON);
            glNormal3fv(cube_normals[poly]);
            glVertex3fv(cube_vertices[cube_polygons[poly][0]]);
            glVertex3fv(cube_vertices[cube_polygons[poly][1]]);
            glVertex3fv(cube_vertices[cube_polygons[poly][2]]);
            glVertex3fv(cube_vertices[cube_polygons[poly][3]]);
        glEnd();
    }
}


float Cube::cube_vertices[9][3] = {                                   /* specifies the (x,y,z)   */
                                  { -1.0,  1.0,  1.0 },    /* 0 */    /* location of each vertex */
                                  { -1.0, -1.0,  1.0 },    /* 1 */
                                  {  1.0, -1.0,  1.0 },    /* 2 */
                                  {  1.0,  1.0,  1.0 },    /* 3 */
                                  {  1.0,  1.0, -1.0 },    /* 4 */
                                  {  1.0, -1.0, -1.0 },    /* 5 */
                                  { -1.0, -1.0, -1.0 },    /* 6 */
                                  { -1.0,  1.0, -1.0 },    /* 7 */
                                  {  0.0,  1.0,  0.0 }     /* 8 */
};


int Cube::cube_polygons[6][4] = {                                  /* specifices the vertices */
                                { 0, 1, 2, 3 },    /* front  */    /* of each polygon         */
                                { 3, 2, 5, 4 },    /* right  */
                                { 5, 6, 7, 4 },    /* back   */
                                { 6, 1, 0, 7 },    /* left   */
                                { 5, 2, 1, 6 },    /* bottom */
                                { 0, 3, 4, 7 }     /* top    */
};


float Cube::cube_normals[6][3] = {                               /* specifies the normal */
                                 {  0.0,  0.0,  1.0 }, /* front  ** of each polygon      */
                                 {  1.0,  0.0,  0.0 }, /* right  */
                                 {  0.0,  0.0, -1.0 }, /* back   */
                                 { -1.0,  0.0,  0.0 }, /* left   */
                                 {  0.0, -1.0,  0.0 }, /* bottom */
                                 {  0.0,  1.0,  0.0 }  /* top    */
};
