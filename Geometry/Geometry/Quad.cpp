#include "Quad.h"

using namespace MTF;

Quad::Quad(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, double r, double g, double b)
{
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    this->v4 = v4;
    this->r = r;
    this->g = g;
    this->b = b;
}


Quad::~Quad()
{
}
