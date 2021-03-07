#include "GeoModule.h"

using namespace MTF;

GeoModule* currentModuleInstance;


bool GeoModule::GetWandPos(Wand wand, int &x, int &y, int windowWidth, int windowHeight)
{
    x = y = -1;

    // Find intersection of wand and full screen quad at screen depth
    Vector3 LL = Vector3(-6.6667, 0.0, 0.0);
    Vector3 LR = Vector3( 6.6667, 0.0, 0.0);
    Vector3 UL = Vector3(-6.6667, 7.5, 0.0);

    double screenWidth = (LR - LL).GetLength();
    double screenHeight = (UL - LL).GetLength();

    Vector3 pos = wand.GetPosition();
    Vector3 view = wand.GetViewVector();

    if (view.GetZ() > 0)
    {
        Vector3 ip = pos + view * fabs(pos.GetZ() / view.GetZ());
        double xPos = ip.GetX() - LL.GetX();
        double yPos = ip.GetY() - LL.GetY();

        if (xPos > 0 && xPos < screenWidth && yPos > 0 && yPos < screenHeight)
        {
            x = (int)((xPos / screenWidth) * windowWidth + 0.5);
            y = (int)((yPos / screenHeight) * windowHeight + 0.5);

            return true;
        }
    }

    return false;
}


void GeoModule::DrawCylinder(Vector3 position, Vector3 view, double length, double radius)
{
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    double subdivisions = 10;

    Vector3 start = position;
    Vector3 end = position + view * length;
    Vector3 oldDir = Vector3(0.0, 0.0, 1.0);
    Vector3 direction = (end - start).GetNormalized();

    double rotationAngle = acos(oldDir.DotProduct(direction)) * 180 / 3.14159;
    Vector3 rotationAxis = oldDir.CrossProduct(direction);

    glPushMatrix();
        glTranslatef(start.GetX(), start.GetY(), start.GetZ());
        glRotatef(rotationAngle, rotationAxis.GetX(), rotationAxis.GetY(), rotationAxis.GetZ());
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluCylinder(quadric, radius, radius, length, subdivisions, 1);

        //draw the first cap
        gluQuadricOrientation(quadric,GLU_INSIDE);
        gluDisk( quadric, 0.0, radius, subdivisions, 1);
        glTranslatef(0, 0, length);

        //draw the second cap
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluDisk(quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}