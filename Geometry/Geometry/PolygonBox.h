#ifndef _POLYGONBOX_H
#define _POLYGONBOX_H

#include "Polygon3D.h"

class PolygonBox : public Polygon3D
{

public:
    PolygonBox(MTF::Vector3 position, double length, double width, double height);
    ~PolygonBox();

    void Draw();
    void Draw(int highlightIndex, std::vector<int> selectedIndex);

    double GetRadius();

    void AddEdge(int v1, int v2);
    void AddTriangle(int v1, int v2, int v3);
    void Clear();

private:
    void *_font;

    double _radius;
    double _length;
    double _width;
    double _height;

    void DrawTriangles(bool DrawSort=false);

    std::vector<MTF::Vector3> _addedEdges;
    std::vector<Face*> _addedTriangles;

    void CreateBox();
};

#endif