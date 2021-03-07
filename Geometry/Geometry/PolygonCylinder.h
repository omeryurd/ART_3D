#ifndef _POLYGONCYLINDER_H
#define _POLYGONCYLINDER_H

#include "Polygon3D.h"

class PolygonCylinder : public Polygon3D
{

public:
    PolygonCylinder(MTF::Vector3 position, double length, double radius, int quality);
    ~PolygonCylinder();

    void Draw();

private:
    double _length;
    double _radius;
    int _quality;

    void CreateCylinder();

    void DrawTriangles(bool DrawSort=false);
};

#endif