#ifndef _PLANE_H
#define _PLANE_H

#include <Windows.h>
#include <GL/glut.h>

#include <Matrix4.h>

#include "LineSegment.h"
#include "Face.h"

#include <iostream>

#define PI 3.14159265

class Plane
{
public:
    Plane();
    Plane(MTF::Vector3 _offset, double _pitch, double _yaw, double radius);
    Plane(MTF::Vector3 _offset, double _radius);
    ~Plane();

    bool GetIntersectionLineSegment(Face face, LineSegment &lineSegment);
    bool GetIntersectionRay(MTF::Vector3 origin, MTF::Vector3 dir, MTF::Vector3 &intersectPoint);

    void Render();
    void Render(double xRot, double yRot);

    MTF::Matrix4 GetRotationMatrix();

    MTF::Vector3 GetNormalVector();
    MTF::Vector3 GetRightVector();
    MTF::Vector3 GetUpVector();

    MTF::Vector3 GetCenter();
    void SetCenter(MTF::Vector3 _center);

    double GetPitch();
    void SetPitch(double _pitch);

    double GetYaw();
    void SetYaw(double _yaw);

    double GetRadius();
    void SetRadius(double _radius);

    double GetDistanceToOriginAlongNormal();

private:
    MTF::Vector3 offset;
    MTF::Vector3 center;
    double pitch;
    double yaw;
    double radius;

    MTF::Matrix4 rotation;

    MTF::Vector3 normalVector;
    MTF::Vector3 upVector;
    MTF::Vector3 rightVector;

    double distanceToOriginAlongNormal;

    double GetDistance(MTF::Vector3 point);

    void CalculateRotationMatrix();
};

#endif