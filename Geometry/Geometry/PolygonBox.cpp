#include "PolygonBox.h"

using namespace MTF;

PolygonBox::PolygonBox(Vector3 position, double length, double width, double height) : Polygon3D(position, 1.0)
{
    _polygonName = "Rectangular Prism";

    _length = length;
    _width = width;
    _height = height;

    _font = GLUT_STROKE_MONO_ROMAN;
    _radius = .2;

    CreateBox();
}


PolygonBox::~PolygonBox()
{
}


void PolygonBox::Draw()
{
    DrawCheckerBoard();

    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glRotatef(_rotationX, 1.0, 0.0, 0.0);
        glRotatef(_rotationY, 0.0, 1.0, 0.0);
        glRotatef(_rotationZ, 0.0, 0.0, 1.0);

        glColor4f(0.25, 0.4, 0.9, 1.0);
        for (int i = 0; i < _edgeList.size(); i += 2)
        {
            DrawCylinder(_edgeList.at(i), _edgeList.at(i+1), 0.05);
        }

        glColor4f(0.2, 0.8, 0.2, 1.0);
        for (int i = 0; i < _vertexList.size(); ++i)
        {
            DrawSphere(_vertexList.at(i), _radius / 2);
        }
    glPopMatrix();

    DrawSort();

    /*
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
    */
    
    DrawName();
}


void PolygonBox::Draw(int highlightIndex, std::vector<int> selectedIndex)
{
    DrawCheckerBoard();

    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glColor4f(1.0, 1.0, 1.0, 1.0);
        for (int i = 0; i < _labelPositionList.size(); ++i)
        {
            glPushMatrix();
                glRotatef(_rotationX, 1.0, 0.0, 0.0);
                glRotatef(_rotationY, 0.0, 1.0, 0.0);
                glRotatef(_rotationZ, 0.0, 0.0, 1.0);
                glTranslatef(_labelPositionList[i].GetX(), _labelPositionList[i].GetY(), _labelPositionList[i].GetZ());
                glRotatef(_rotationZ, 0.0, 0.0, -1.0);
                glRotatef(_rotationY, 0.0, -1.0, 0.0);
                glRotatef(_rotationX, -1.0, 0.0, 0.0);
                glScalef(0.004, 0.004, 0.004);
                glutStrokeCharacter(_font, (char)('A' + i));
            glPopMatrix();
        }

        glRotatef(_rotationX, 1.0, 0.0, 0.0);
        glRotatef(_rotationY, 0.0, 1.0, 0.0);
        glRotatef(_rotationZ, 0.0, 0.0, 1.0);

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
    glPopMatrix();

    DrawSort();
    /*
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
    */

    DrawName();
}


void PolygonBox::DrawTriangles(bool DrawSort)
{
    glPushMatrix();
        glTranslated(_position.GetX(), _position.GetY(), _position.GetZ());

        glRotatef(_rotationX, 1.0, 0.0, 0.0);
        glRotatef(_rotationY, 0.0, 1.0, 0.0);
        glRotatef(_rotationZ, 0.0, 0.0, 1.0);

        glColor4f(1.0, 1.0, 1.0, _transparency);
        glBegin(GL_TRIANGLES);
            int numFaces = _mesh.size();
            for (int m = 0; m < numFaces; ++m)
            {
                int i = m;
                if (DrawSort)
                    i = triangles[numFaces - m - 1].index;

                Face *f = _mesh.at(i);
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        glEnd();
    glPopMatrix();
}


double PolygonBox::GetRadius()
{
    return _radius;
}


void PolygonBox::CreateBox()
{
    double l = _length / 2;
    double w = _width / 2;
    double h = _height / 2;

    // Create Vertex List
    _vertexList.push_back(Vector3(-l,  h,  w)); // 0
    _vertexList.push_back(Vector3(-l, -h,  w)); // 1
    _vertexList.push_back(Vector3( l, -h,  w)); // 2
    _vertexList.push_back(Vector3( l,  h,  w)); // 3
    _vertexList.push_back(Vector3( l,  h, -w)); // 4
    _vertexList.push_back(Vector3( l, -h, -w)); // 5
    _vertexList.push_back(Vector3(-l, -h, -w)); // 6
    _vertexList.push_back(Vector3(-l,  h, -w)); // 7

    // Create Label Position List
    _labelPositionList.push_back(_vertexList[0] * 1.15);
    _labelPositionList.push_back(_vertexList[1] * 1.15);
    _labelPositionList.push_back(_vertexList[2] * 1.15);
    _labelPositionList.push_back(_vertexList[3] * 1.15);
    _labelPositionList.push_back(_vertexList[4] * 1.15);
    _labelPositionList.push_back(_vertexList[5] * 1.15);
    _labelPositionList.push_back(_vertexList[6] * 1.15);
    _labelPositionList.push_back(_vertexList[7] * 1.15);

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
    std::vector<Face*> *currentFace = new std::vector<Face*>();
    std::vector<Face*> *previousFace = new std::vector<Face*>();

    // Initial faces
    currentFace->push_back(new Face(_vertexList.at(0), _vertexList.at(1), _vertexList.at(2)));
    currentFace->push_back(new Face(_vertexList.at(2), _vertexList.at(3), _vertexList.at(0)));
    currentFace->push_back(new Face(_vertexList.at(3), _vertexList.at(2), _vertexList.at(5)));
    currentFace->push_back(new Face(_vertexList.at(5), _vertexList.at(4), _vertexList.at(3)));
    currentFace->push_back(new Face(_vertexList.at(5), _vertexList.at(6), _vertexList.at(7)));
    currentFace->push_back(new Face(_vertexList.at(7), _vertexList.at(4), _vertexList.at(5)));
    currentFace->push_back(new Face(_vertexList.at(6), _vertexList.at(1), _vertexList.at(0)));
    currentFace->push_back(new Face(_vertexList.at(0), _vertexList.at(7), _vertexList.at(6)));
    currentFace->push_back(new Face(_vertexList.at(5), _vertexList.at(2), _vertexList.at(1)));
    currentFace->push_back(new Face(_vertexList.at(1), _vertexList.at(6), _vertexList.at(5)));
    currentFace->push_back(new Face(_vertexList.at(0), _vertexList.at(3), _vertexList.at(4)));
    currentFace->push_back(new Face(_vertexList.at(4), _vertexList.at(7), _vertexList.at(0)));

    // Subdivide the box faces into smaller triangles
    for (int i = 0; i < 4; ++i)
    {
        // Prepare for a new current face list, discard the old
        delete previousFace;
        previousFace = currentFace;
        currentFace = new std::vector<Face*>();
        
        for (int k = 0; k < previousFace->size(); ++k)
        {
            Vector3 p0 = previousFace->at(k)->_point[0];
            Vector3 p1 = previousFace->at(k)->_point[1];
            Vector3 p2 = previousFace->at(k)->_point[2];

            Vector3 p01 = ((p0 + p1) / 2);
            Vector3 p12 = ((p1 + p2) / 2);
            Vector3 p02 = ((p0 + p2) / 2);

            currentFace->push_back(new Face(p0, p01, p02));
            currentFace->push_back(new Face(p1, p12, p01));
            currentFace->push_back(new Face(p2, p02, p12));
            currentFace->push_back(new Face(p01, p12, p02));
        }
    }

    for (int i = 0; i < currentFace->size(); ++i)
    {
        _mesh.push_back(currentFace->at(i));
    }

    delete currentFace, previousFace;
}


void PolygonBox::AddEdge(int v1, int v2)
{
    _addedEdges.push_back(_vertexList[v1]);
    _addedEdges.push_back(_vertexList[v2]);
}


void PolygonBox::AddTriangle(int v1, int v2, int v3)
{
    _addedTriangles.push_back(new Face(_vertexList[v1], _vertexList[v2], _vertexList[v3]));
}


void PolygonBox::Clear()
{
    _addedEdges.clear();
    _addedTriangles.clear();
}
