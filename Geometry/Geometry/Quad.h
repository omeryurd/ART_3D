#ifndef _QUAD_H
#define _QUAD_H

#include <Vector3.h>

class Quad
{
public:
    Quad(MTF::Vector3 v1, MTF::Vector3 v2, MTF::Vector3 v3, MTF::Vector3 v4, double r, double g, double b);
    ~Quad(void);

    MTF::Vector3 v1;
    MTF::Vector3 v2;
    MTF::Vector3 v3;
    MTF::Vector3 v4;

    double r;
    double g;
    double b;
};

#endif