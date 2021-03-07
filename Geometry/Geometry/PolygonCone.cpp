#include "PolygonCone.h"

using namespace MTF;

PolygonCone::PolygonCone(Vector3 position, double length, double radius, int quality) : Polygon3D(position, 1.0)
{
    _polygonName = "Cone";

    _length = length;
    _radius = radius;
    _quality = quality;

    CreateCone();
}


PolygonCone::~PolygonCone()
{
}


void PolygonCone::DrawTriangles(bool DrawSort)
{
    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glRotatef(_rotationX, 1.0, 0.0, 0.0);
        glRotatef(_rotationY, 0.0, 1.0, 0.0);
        glRotatef(_rotationZ, 0.0, 0.0, 1.0);

        glDisable(GL_TEXTURE_2D);

        glBegin(GL_TRIANGLES);
            int numFaces = _mesh.size();
            for (int m = 0; m < numFaces; ++m)
            {
                int i = m;
                if (DrawSort)
                    i = triangles[numFaces - m - 1].index;

                if (i % 4 == 2)
                    glColor4f(1.0, 1.0, 1.0, _transparency);
                else
                    glColor4f(0.0, 0.0, 1.0, _transparency);

                Face *f = _mesh.at(i);
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        glEnd();
    glPopMatrix();
}


void PolygonCone::Draw()
{
    DrawCheckerBoard();

    if (_transparency == 1.0)
    {
        DrawTriangles();
    }
    else
    {
        DrawSort();
    }
    
    DrawName();
}


void PolygonCone::CreateCone()
{
    // Create Vertex List
    //   Base of the Cone
    double y = -_length / 2.0;

    //   Tip of the cone
    _vertexList.push_back(Vector3(0, -y, 0));
    //   Bottom of the cone
    _vertexList.push_back(Vector3(0, y, 0));

    double PI = 3.14159;

    for (int i = 0; i < _quality; ++i)
    {
        double x = _radius * cos(2 * i * PI / _quality);
        double z = _radius * sin(2 * i * PI / _quality);	

        _vertexList.push_back(Vector3(x, y, z));
    }

    // Create Mesh
    for (int i = 1; i < _quality; ++i)
    {
        _mesh.push_back(new Face(_vertexList[0], _vertexList[i+1], _vertexList[i+2]));
        _mesh.push_back(new Face(_vertexList[1], _vertexList[i+2], _vertexList[i+1]));
    }

    _mesh.push_back(new Face(_vertexList[0], _vertexList[_quality+1], _vertexList[2]));
    _mesh.push_back(new Face(_vertexList[1], _vertexList[2], _vertexList[_quality+1]));

}
