#include "PolygonCylinder.h"

using namespace MTF;

PolygonCylinder::PolygonCylinder(Vector3 position, double length, double radius, int quality) : Polygon3D(position, 1.0)
{
    _polygonName = "Cone";

    _length = length;
    _radius = radius;
    _quality = quality;

    CreateCylinder();
}


PolygonCylinder::~PolygonCylinder()
{
}


void PolygonCylinder::DrawTriangles(bool DrawSort)
{
    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glRotatef(_rotationX, 1.0, 0.0, 0.0);
        glRotatef(_rotationY, 0.0, 1.0, 0.0);
        glRotatef(_rotationZ, 0.0, 0.0, 1.0);

        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0, 1.0, 1.0, _transparency);
        glBegin(GL_TRIANGLES);
            int numFaces = _mesh.size();
            for (int m = 0; m < numFaces; ++m)
            {
                int i = m;
                if (DrawSort)
                    i = triangles[numFaces - m - 1].index;

                if (i <= _quality * 2)
                {
                    if (i % 4 == 0 || i % 4 == 1)
                        glColor4f(0.0, 0.0, 1.0, _transparency);
                    else
                        glColor4f(1.0, 1.0, 1.0, _transparency);
                }
                else {
                    if (i % 2 == 0)
                        glColor4f(0.0, 0.0, 1.0, _transparency);
                    else
                        glColor4f(1.0, 1.0, 1.0, _transparency);
                }

                Face *f = _mesh.at(i);
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        glEnd();
    glPopMatrix();
}


void PolygonCylinder::Draw()
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


void PolygonCylinder::CreateCylinder()
{
    // Create Vertex List
    //   Topp of the cylinder
    _vertexList.push_back(Vector3(0, _length / 2, 0));
    //   Bottom of the cylinder
    _vertexList.push_back(Vector3(0, -_length / 2, 0));

    
    double PI = 3.14159;

    for (int i = 0; i < _quality; ++i)
    {
        double x = _radius * cos(2 * i * PI / _quality);
        double z = _radius * sin(2 * i * PI / _quality);	

        _vertexList.push_back(Vector3(x, _length / 2.0, z));  // top circle
        _vertexList.push_back(Vector3(x, -_length / 2.0, z)); // bottom circle
    }

    // Create Mesh cylinder
    for (int i = 0; i < _quality - 1; ++i)
    {
        _mesh.push_back(new Face(_vertexList[2 + 2*i], _vertexList[2 + 2*i + 3], _vertexList[2 + 2*i + 1]));
        _mesh.push_back(new Face(_vertexList[2 + 2*i], _vertexList[2 + 2*i + 2], _vertexList[2 + 2*i + 3]));
    }
    _mesh.push_back(new Face(_vertexList[2 * _quality], _vertexList[3], _vertexList[2 * _quality + 1]));
    _mesh.push_back(new Face(_vertexList[2 * _quality], _vertexList[2], _vertexList[3]));

    // Create top and bottom end caps
    for (int i = 0; i < _quality - 1; ++i)
    {
        _mesh.push_back(new Face(_vertexList[0], _vertexList[2 + 2*i], _vertexList[2 + 2*i + 2]));
        _mesh.push_back(new Face(_vertexList[1], _vertexList[2 + 2*i + 1], _vertexList[2 + 2*i + 3]));
    }
    _mesh.push_back(new Face(_vertexList[0], _vertexList[2 * _quality], _vertexList[2]));
    _mesh.push_back(new Face(_vertexList[1], _vertexList[2 * _quality + 1], _vertexList[3]));
}
