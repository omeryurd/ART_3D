#ifndef _POLYGONSPHERE_H
#define _POLYGONSPHERE_H

#include "Polygon3D.h"

class PolygonSphere : public Polygon3D
{

public:
    PolygonSphere(MTF::Vector3 position, double radius, int quality);
    ~PolygonSphere();

    void Draw();

private:
    double _radius;
    int _quality;

    void CreateSphere();

    void DrawTriangles(bool DrawSort=false);
};

#endif