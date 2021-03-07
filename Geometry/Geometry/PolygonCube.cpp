#include "PolygonCube.h"

using namespace MTF;

PolygonCube::PolygonCube(Vector3 position, double scale) : Polygon(position, scale)
{
    _polygonName = "Pythagorean Box";
    _radius = .1;
    CreateCube();
}


PolygonCube::~PolygonCube()
{
}


void PolygonCube::Draw()
{
    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glColor4f(0.25, 0.4, 0.9, 1.0);
        for (int i = 0; i < _edgeList.size(); i += 2)
        {
            DrawCylinder(_edgeList.at(i), _edgeList.at(i+1), 0.05);
        }

        glColor4f(0.2, 0.8, 0.2, 1.0);
        for (int i = 0; i < _vertexList.size(); ++i)
        {
            DrawSphere(_vertexList.at(i), _radius);
        }

        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0, 1.0, 1.0, _transparency);
        glBegin(GL_TRIANGLES);
            int numFaces = _mesh.size();
            for (int i = 0; i < numFaces; ++i)
            {
                Face *f = _mesh.at(i);
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        glEnd();
    glPopMatrix();

    DrawName();
}


void PolygonCube::Draw(int highlightIndex, std::vector<int> selectedIndex)
{
    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glColor4f(0.25, 0.4, 0.9, 1.0);
        for (int i = 0; i < _edgeList.size(); i += 2)
        {
            DrawCylinder(_edgeList.at(i), _edgeList.at(i+1), 0.05);
        }

        glColor4f(0.071, 0.04, 0.56, 1.0);
        for (int i = 0; i < _addedEdges.size(); i += 2)
        {
            DrawCylinder(_addedEdges.at(i), _addedEdges.at(i+1), 0.05);
        }

        for (int i = 0; i < _vertexList.size(); ++i)
        {
            glColor4f(0.2, 0.8, 0.2, 1.0);

            if (i == highlightIndex)
            {
                glColor4f(0.8, 0.0, 0.8, 1.0);
            }
            else
            {
                for (int j = 0; j < selectedIndex.size(); ++j)
                {
                    if (i == selectedIndex.at(j))
                    {
                        glColor4f(0.8, 0.0, 0.0, 1.0);
                    }
                }
            }

            DrawSphere(_vertexList.at(i), _radius);
        }

        glColor4f(1.0, 0.73, 0.0, 1.0);
        glBegin(GL_TRIANGLES);
        {
            int numFaces = _addedTriangles.size();
            for (int i = 0; i < numFaces; ++i)
            {
                Face *f = _addedTriangles.at(i);
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        }
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
        glColor4f(1.0, 1.0, 1.0, _transparency);
        glBegin(GL_TRIANGLES);
        {
            int numFaces = _mesh.size();
            for (int i = 0; i < numFaces; ++i)
            {
                Face *f = _mesh.at(i);
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        }
        glEnd();
    glPopMatrix();

    DrawName();
}


void PolygonCube::DrawCuttingPlane()
{
}


double PolygonCube::GetRadius()
{
    return _radius;
}


void PolygonCube::CreateCube()
{
    // Create Vertex List
    _vertexList.push_back(Vector3(-1.0,  1.0,  1.0)); // 0
    _vertexList.push_back(Vector3(-1.0, -1.0,  1.0)); // 1
    _vertexList.push_back(Vector3( 1.0, -1.0,  1.0)); // 2
    _vertexList.push_back(Vector3( 1.0,  1.0,  1.0)); // 3
    _vertexList.push_back(Vector3( 1.0,  1.0, -1.0)); // 4
    _vertexList.push_back(Vector3( 1.0, -1.0, -1.0)); // 5
    _vertexList.push_back(Vector3(-1.0, -1.0, -1.0)); // 6
    _vertexList.push_back(Vector3(-1.0,  1.0, -1.0)); // 7

    // Create Edge List
    _edgeList.push_back(_vertexList.at(0));
    _edgeList.push_back(_vertexList.at(1));
    _edgeList.push_back(_vertexList.at(0));
    _edgeList.push_back(_vertexList.at(3));
    _edgeList.push_back(_vertexList.at(0));
    _edgeList.push_back(_vertexList.at(7));
    _edgeList.push_back(_vertexList.at(1));
    _edgeList.push_back(_vertexList.at(2));
    _edgeList.push_back(_vertexList.at(1));
    _edgeList.push_back(_vertexList.at(6));
    _edgeList.push_back(_vertexList.at(2));
    _edgeList.push_back(_vertexList.at(3));
    _edgeList.push_back(_vertexList.at(2));
    _edgeList.push_back(_vertexList.at(5));
    _edgeList.push_back(_vertexList.at(3));
    _edgeList.push_back(_vertexList.at(4));
    _edgeList.push_back(_vertexList.at(4));
    _edgeList.push_back(_vertexList.at(5));
    _edgeList.push_back(_vertexList.at(4));
    _edgeList.push_back(_vertexList.at(7));
    _edgeList.push_back(_vertexList.at(5));
    _edgeList.push_back(_vertexList.at(6));
    _edgeList.push_back(_vertexList.at(6));
    _edgeList.push_back(_vertexList.at(7));

    // Create Mesh
    _mesh.push_back(new Face(_vertexList.at(0), _vertexList.at(1), _vertexList.at(2)));
    _mesh.push_back(new Face(_vertexList.at(2), _vertexList.at(3), _vertexList.at(0)));
    _mesh.push_back(new Face(_vertexList.at(3), _vertexList.at(2), _vertexList.at(5)));
    _mesh.push_back(new Face(_vertexList.at(5), _vertexList.at(4), _vertexList.at(3)));
    _mesh.push_back(new Face(_vertexList.at(5), _vertexList.at(6), _vertexList.at(7)));
    _mesh.push_back(new Face(_vertexList.at(7), _vertexList.at(4), _vertexList.at(5)));
    _mesh.push_back(new Face(_vertexList.at(6), _vertexList.at(1), _vertexList.at(0)));
    _mesh.push_back(new Face(_vertexList.at(0), _vertexList.at(7), _vertexList.at(6)));
    _mesh.push_back(new Face(_vertexList.at(5), _vertexList.at(2), _vertexList.at(1)));
    _mesh.push_back(new Face(_vertexList.at(1), _vertexList.at(6), _vertexList.at(5)));
    _mesh.push_back(new Face(_vertexList.at(0), _vertexList.at(3), _vertexList.at(4)));
    _mesh.push_back(new Face(_vertexList.at(4), _vertexList.at(7), _vertexList.at(0)));
}


void PolygonCube::AddEdge(int v1, int v2)
{
    _addedEdges.push_back(_vertexList[v1]);
    _addedEdges.push_back(_vertexList[v2]);
}


void PolygonCube::AddTriangle(int v1, int v2, int v3)
{
    _addedTriangles.push_back(new Face(_vertexList[v1], _vertexList[v2], _vertexList[v3]));
}


void PolygonCube::Clear()
{
    _addedEdges.clear();
    _addedTriangles.clear();
}