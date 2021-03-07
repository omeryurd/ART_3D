#include "LineSegment.h"

using namespace MTF;

LineSegment::LineSegment(Vector3 _point1, Vector3 _point2)
{
    point1 = _point1;
    point2 = _point2;
}


LineSegment::~LineSegment()
{
}


std::string LineSegment::ToString()
{
    std::stringstream ss;
    ss << "Point1: (" << point1.GetX() << ", " << point1.GetY() << ", " << point1.GetZ() << ")\tPoint2: (" << point2.GetX() << ", " << point2.GetY() << ", " << point2.GetZ() << ")" << std::endl;
    return ss.str();
}


void LineSegment::Render()
{
    glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor4f(0.0, 1.0, 0.0, 1.0);

        glBegin(GL_TRIANGLES);
            glVertex3d(point1.GetX(), point1.GetY(), point1.GetZ());
            glVertex3d(0, 0, 0);
            glVertex3d(point2.GetX(), point2.GetY(), point2.GetZ());
        glEnd();
    glPopMatrix();
}