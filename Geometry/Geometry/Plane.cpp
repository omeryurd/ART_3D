#include "Plane.h"

#define epsilon 0.000001

using namespace MTF;

Plane::Plane()
{
    center = Vector3::ZERO;
    pitch = yaw = 0;
    radius = 3;

    CalculateRotationMatrix();
}


Plane::Plane(Vector3 _offset, double _pitch, double _yaw, double _radius)
{
    center = Vector3::ZERO;
    offset = _offset;
    pitch = _pitch * PI / 180;
    yaw = _yaw * PI / 180;
    radius = _radius;

    CalculateRotationMatrix();
}

Plane::Plane(Vector3 _offset, double _radius)
{
    center = Vector3::ZERO;
    offset = _offset;
    radius = _radius;
    pitch = 0;
    yaw = 0;

    CalculateRotationMatrix();
}


Plane::~Plane()
{
}


bool Plane::GetIntersectionLineSegment(Face face, LineSegment &lineSegment)
{
    bool intersect = false;

    Vector3 p0 = face._point[0];
    Vector3 p1 = face._point[1];
    Vector3 p2 = face._point[2];

    // Get the distance of each point from the plane, along the normal of the plane
    // if the distance is 0, it is on the plane, if they have opposite signs, they
    // are on opposite sides of the plane, and we have an intersection
    double p0Dist = GetDistance(p0);
    double p1Dist = GetDistance(p1);
    double p2Dist = GetDistance(p2);

    // Check if p0 and p1 are on the plane
    if (p0Dist == 0 && p1Dist == 0 && p2Dist != 0)
    {
        lineSegment.point1 = p0;
        lineSegment.point2 = p1;
        intersect = true;
    }
    // Check if p0 and p2 are on the plane
    else if (p0Dist == 0 && p2Dist == 0 && p1Dist != 0)
    {
        lineSegment.point1 = p0;
        lineSegment.point2 = p2;
        intersect = true;
    }
    // Check if p1 and p2 are on the plane
    else if (p1Dist == 0 && p2Dist == 0 && p0Dist != 0)
    {
        lineSegment.point1 = p1;
        lineSegment.point2 = p2;
        intersect = true;
    }

    // Check if P0 is on its own side
    else if ((p0Dist >= 0 && p1Dist <= 0 && p2Dist <= 0) || (p0Dist <= 0 && p1Dist >= 0 && p2Dist >= 0))
    {
        lineSegment.point1 = p0 + (p2 - p0) * (normalVector.DotProduct(center - p0) / normalVector.DotProduct(p2 - p0));
        lineSegment.point2 = p0 + (p1 - p0) * (normalVector.DotProduct(center - p0) / normalVector.DotProduct(p1 - p0));
        intersect = true;
    }
    // Check if P1 is on its own side
    else if ((p1Dist >= 0 && p0Dist <= 0 && p2Dist <= 0) || (p1Dist <= 0 && p0Dist >= 0 && p2Dist >= 0))
    {
        lineSegment.point1 = p1 + (p2 - p1) * (normalVector.DotProduct(center - p1) / normalVector.DotProduct(p2 - p1));
        lineSegment.point2 = p1 + (p0 - p1) * (normalVector.DotProduct(center - p1) / normalVector.DotProduct(p0 - p1));
        intersect = true;
    }
    // Check if P2 is on its own side
    else if ((p2Dist >= 0 && p0Dist <= 0 && p1Dist <= 0) || (p2Dist <= 0 && p0Dist >= 0 && p1Dist >= 0))
    {
        lineSegment.point1 = p2 + (p0 - p2) * (normalVector.DotProduct(center - p2) / normalVector.DotProduct(p0 - p2));
        lineSegment.point2 = p2 + (p1 - p2) * (normalVector.DotProduct(center - p2) / normalVector.DotProduct(p1 - p2));
        intersect = true;
    }

    return intersect;
}


bool Plane::GetIntersectionRay(Vector3 origin, Vector3 dir, Vector3 &intersectPoint)
{
    Vector3 start = origin;
    Vector3 end = origin + dir * 500;

    // Get the distance of each point from the plane, along the normal of the plane
    // if the distance is 0, it is on the plane, if they have opposite signs, they
    // are on opposite sides of the plane, and we have an intersection
    double startDist = GetDistance(start);
    double endDist = GetDistance(end);

    // Check if the line is on the plane (we aren't going to bother calling this an intersection)
    if (startDist == 0 && endDist == 0)
    {
        return false;
    }
    // Check if they are on opposite sides
    if ((startDist > 0 && endDist < 0) || (startDist < 0 && endDist > 0))
    {
        intersectPoint = start + (end - start) * (normalVector.DotProduct(center - start) / normalVector.DotProduct(end - start));
        std::cout << "IP: " << intersectPoint.ToString() << "Center: " << (center + offset).ToString() << std::endl;

        // Check if the intersection is inside the radius
        if (intersectPoint.GetDistance(center + offset) <= radius)
            return true;
    }

    return false;
}


void Plane::Render()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef((offset + center).GetX() , (offset + center).GetY(), (offset + center).GetZ());
        glRotatef(pitch * 180 / PI, 1.0, 0.0, 0.0);
        glRotatef(yaw * 180 / PI, 0.0, 1.0, 0.0);

        glBegin(GL_TRIANGLES);
            float x = (float)radius * cos(355 * PI/180.0f);
            float y = (float)radius * sin(355 * PI/180.0f);

            for (int j = 0; j < 360; j += 5)
            {
                glVertex2f(x,y);
                glVertex2f(0,0);
                x = (float)radius * cos(j * PI/180.0f);
                y = (float)radius * sin(j * PI/180.0f);
                glVertex2f(x, y);
            }
        glEnd();
    glPopMatrix();
}


void Plane::Render(double xRot, double yRot)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPushMatrix();
        glTranslatef((offset + center).GetX() , (offset + center).GetY(), (offset + center).GetZ());
        glRotatef(xRot, 1.0, 0.0, 0.0);
        glRotatef(yRot, 0.0, 1.0, 0.0);
        glRotatef(pitch * 180 / PI, 1.0, 0.0, 0.0);
        glRotatef(yaw * 180 / PI, 0.0, 1.0, 0.0);

        glBegin(GL_TRIANGLES);
            float x = (float)radius * cos(355 * PI/180.0f);
            float y = (float)radius * sin(355 * PI/180.0f);

            for (int j = 0; j < 360; j += 5)
            {
                glVertex2f(x,y);
                glVertex2f(0,0);
                x = (float)radius * cos(j * PI/180.0f);
                y = (float)radius * sin(j * PI/180.0f);
                glVertex2f(x, y);
            }
        glEnd();
    glPopMatrix();
}


Vector3 Plane::GetNormalVector()
{
    return normalVector;
}


Vector3 Plane::GetRightVector()
{
    return rightVector;
}


Vector3 Plane::GetUpVector()
{
    return upVector;
}


Matrix4 Plane::GetRotationMatrix()
{
    return rotation;
}


Vector3 Plane::GetCenter()
{
    return center;
}


void Plane::SetCenter(Vector3 _center)
{
    center = _center;
    CalculateRotationMatrix();
}


double Plane::GetPitch()
{
    return pitch * 180 / PI;
}


void Plane::SetPitch(double _pitch)
{
    pitch = _pitch * PI / 180;
    CalculateRotationMatrix();
}


double Plane::GetYaw()
{
    return yaw * 180 / PI;
}


void Plane::SetYaw(double _yaw)
{
    yaw = _yaw * PI / 180;
    CalculateRotationMatrix();
}


double Plane::GetRadius()
{
    return radius;
}
 

void Plane::SetRadius(double _radius)
{
    radius = _radius;
}


double Plane::GetDistance(Vector3 point)
{
    return normalVector.DotProduct(point - center);
}


double Plane::GetDistanceToOriginAlongNormal()
{
    return distanceToOriginAlongNormal;
}


void Plane::CalculateRotationMatrix()
{
    // Make pitch and yaw fall between 0 and 2*PI
    while (pitch < 0)
        pitch += 2*PI;
    while (pitch >= 2*PI)
        pitch -= 2*PI;

    while (yaw < 0)
        yaw += 2*PI;
    while (yaw >= 2*PI)
        yaw -= 2*PI;

    // Create a rotation matrix from the pitch and yaw
    Matrix4 pitchMatrix;
    Matrix4 yawMatrix;

    pitchMatrix.MakePitchRotationMatrix(pitch);
    yawMatrix.MakeYawRotationMatrix(yaw);

    //rotation = yawMatrix * pitchMatrix;
    rotation = pitchMatrix * yawMatrix;

    // Update the vectors defining the plane
    //normalVector = (rotation * Vector3::NEGATIVE_UNIT_Z).GetNormalized();
    normalVector = (rotation * Vector3::UNIT_Z).GetNormalized();
    upVector = (rotation * Vector3::UNIT_Y).GetNormalized();
    rightVector = (rotation * Vector3::UNIT_X).GetNormalized();

    // Update distance to origin along the normal (or inverse normal)
    distanceToOriginAlongNormal = normalVector.DotProduct(center) / normalVector.DotProduct(normalVector);
}
