#ifndef _POLYGONCONE_H
#define _POLYGONCONE_H

#include "Polygon3D.h"

class PolygonCone : public Polygon3D
{

public:
    PolygonCone(MTF::Vector3 position, double length, double radius, int quality);
    ~PolygonCone();

    void Draw();

private:
    double _length;
    double _radius;
    int _quality;

    void DrawTriangles(bool DrawSort=false);

    void CreateCone();
};

#endif