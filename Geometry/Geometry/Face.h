#ifndef _FACE_H
#define _FACE_H

#include <Vector3.h>

class Face
{

public:
    MTF::Vector3 _point[3];
    MTF::Vector3 _normal;

    Face(MTF::Vector3 point[3]);
    Face(MTF::Vector3 p1, MTF::Vector3 p2, MTF::Vector3 p3);
    Face(MTF::Vector3 p1, MTF::Vector3 p2, MTF::Vector3 p3, MTF::Vector3 normal);
    ~Face();

    bool Intersect(MTF::Vector3 origin, MTF::Vector3 direction, MTF::Vector3 &intersectPoint);
    
private:

    void CalculateNormal();
};

#endif