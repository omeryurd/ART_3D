#ifndef _POLYGON_H
#define _POLYGON_H

#include <string>
#include <vector>

#include <Windows.h>
#include <GL\glut.h>

#include <Monolith.h>

#include "Face.h"

#include "Quad.h"
#include "Tri.h"

class Polygon3D
{

public:
    std::string _polygonName;

    Polygon3D(MTF::Vector3 position, double scale);
    ~Polygon3D();

    virtual void Draw()=0;

    void AdjustRotationX(double rotation);
    void AdjustRotationY(double rotation);
    void AdjustRotationZ(double rotation);

    void AdjustCuttingPlaneRotationX(double rotation);
    void AdjustCuttingPlaneRotationZ(double rotation);
    void AdjustCuttingPlaneHeightOffset(double offset);

    double GetRotationX();
    double GetRotationY();
    double GetRotationZ();
    
    void SetScale(double scale);
    double GetScale();

    void SetTransparency(double transparency);
    double GetTransparency();

    void Reset();

    void DrawString(int x, int y, std::string str);
    void DrawName();

    void DrawCylinder(MTF::Vector3 start, MTF::Vector3 end, double radius);
    void DrawSphere(MTF::Vector3 center, double radius);

    bool IntersectSpheres(MTF::Vector3 rayOrigin, MTF::Vector3 rayEnd, double radius, int &selectedIndex);

    bool GetMouseRays(int x, int y, MTF::Vector3 &rayOrigin, MTF::Vector3 &rayEnd);

    std::vector<MTF::Vector3> _vertexList;
    std::vector<MTF::Vector3> _edgeList;
    std::vector<MTF::Vector3> _labelPositionList;

    std::vector<Face*> _mesh;

    char GetLabel(int index);

    MTF::Vector3 GetPosition();

    std::vector<Tri> triangles;
    virtual void DrawTriangles(bool DrawSort=false)=0;
    void DrawSort();

    void DrawCheckerBoard();

protected:
    MTF::Vector3 _position;

    double _scale;

    double _rotationX;
    double _rotationY;
    double _rotationZ;

    double _cuttingPlaneRotationX;
    double _cuttingPlaneRotationZ;
    double _cuttingPlaneHeightOffset;

    double _transparency;

private:

    void CreateCheckerBoard();
    std::vector<Quad> _checkerBoard;
};

#endif