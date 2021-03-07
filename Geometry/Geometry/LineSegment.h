#ifndef _LINESEGMENT_H
#define _LINESEGMENT_H

#include <Windows.h>
#include <GL/glut.h>

#include <Vector3.h>

class LineSegment
{
public:
    MTF::Vector3 point1;
    MTF::Vector3 point2;

    LineSegment(MTF::Vector3 _point1, MTF::Vector3 _point2);
    ~LineSegment();

    std::string ToString();
    void Render();
};

#endif