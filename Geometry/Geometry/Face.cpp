#include "Face.h"

using namespace MTF;

Face::Face(Vector3 point[3]) 
{
    _point[0] = point[0];
    _point[1] = point[1];
    _point[2] = point[2];
    CalculateNormal();
}

    
Face::Face(Vector3 p1, Vector3 p2, Vector3 p3)
{
    _point[0] = p1;
    _point[1] = p2;
    _point[2] = p3;
    CalculateNormal();
}


Face::Face(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 normal)
{
    _point[0] = p1;
    _point[1] = p2;
    _point[2] = p3;
    _normal = normal;
}


Face::~Face()
{
}


bool Face::Intersect(Vector3 origin, Vector3 direction, Vector3 &intersectPoint)
{
    bool intersect = false;
    double denominator = _normal.DotProduct(direction);

    // Ray and Plane are not parallel
    if (denominator != 0)
    {
        double length = _normal.DotProduct(_point[0] - origin) / denominator;
        Vector3 ip = origin + direction * length;

        // Perform a test to see if the point on the plane is inside the triangle or not
        // We will use the barycentric approach, since we do not know if we have a CCW or CW ordering
        //
        // Compute vectors        
        Vector3 v0 = _point[2] - _point[0];
        Vector3 v1 = _point[1] - _point[0];
        Vector3 v2 = ip - _point[0];

        // Compute dot products
        double dot00 = v0.DotProduct(v0);
        double dot01 = v0.DotProduct(v1);
        double dot02 = v0.DotProduct(v2);
        double dot11 = v1.DotProduct(v1);
        double dot12 = v1.DotProduct(v2);

        // Compute barycentric coordinates
        double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
        double uC = (dot11 * dot02 - dot01 * dot12) * invDenom;
        double vC = (dot00 * dot12 - dot01 * dot02) * invDenom;

        // Check if point is in triangle
        if ((uC > 0) && (vC > 0) && (uC + vC < 1))
        {
            intersect = true;
            intersectPoint = ip;
        }
    }

    return intersect;
}


void Face::CalculateNormal()
{
    // Calculate Normal (V1 - V0) X (V2 - V0)
    double xV10 = _point[1].GetX() - _point[0].GetX();
    double yV10 = _point[1].GetY() - _point[0].GetY();
    double zV10 = _point[1].GetZ() - _point[0].GetZ();

    double xV20 = _point[2].GetX() - _point[0].GetX();
    double yV20 = _point[2].GetY() - _point[0].GetY();
    double zV20 = _point[2].GetZ() - _point[0].GetZ();

    double xNorm = yV10 * zV20 - zV10 * yV20;
    double yNorm = zV10 * xV20 - xV10 * zV20;
    double zNorm = xV10 * yV20 - yV10 * xV20;

    _normal = Vector3(xNorm, yNorm, zNorm);
}
