#ifndef _POLYGONCUBE_H
#define _POLYGONCUBE_H

#include "Polygon.h"

class PolygonCube : public Polygon
{

public:
    PolygonCube(MTF::Vector3 position, double scale);
    ~PolygonCube();

    void Draw();
    void Draw(int highlightIndex, std::vector<int> selectedIndex);
    void DrawCuttingPlane();

    double GetRadius();

    void AddEdge(int v1, int v2);
    void AddTriangle(int v1, int v2, int v3);
    void Clear();

private:
    double _radius;

    std::vector<MTF::Vector3> _addedEdges;
    std::vector<Face*> _addedTriangles;

    void CreateCube();
};

#endif