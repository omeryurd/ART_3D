#include "PolygonSphere.h"

using namespace MTF;


PolygonSphere::PolygonSphere(Vector3 position, double radius, int quality) : Polygon3D(position, 1.0)
{
    _polygonName = "Sphere";

    _radius = radius;
    _quality = quality;

    CreateSphere();
}


PolygonSphere::~PolygonSphere()
{
}


void PolygonSphere::DrawTriangles(bool DrawSort)
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

                Face *f = _mesh.at(i);
                if (i * 80 / numFaces % 4 == 3)
                {
                    glColor4f(0.0, 0.0, 1.0, _transparency);
                    if (i * 320 / numFaces % 4 == 3)
                        glColor4f(1.0, 1.0, 1.0, _transparency);
                }
                else
                {
                    glColor4f(1.0, 1.0, 1.0, _transparency);
                }
                for (int j = 0; j < 3; ++j)
                {
                    glVertex3d(f->_point[j].GetX(), f->_point[j].GetY(), f->_point[j].GetZ());
                }
            }
        glEnd();
    glPopMatrix();
}


void PolygonSphere::Draw()
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


void PolygonSphere::CreateSphere()
{
    std::vector<Vector3> *currentVertex = new std::vector<Vector3>();
    std::vector<Vector3> *previousVertex = new std::vector<Vector3>();
    std::vector<Face*> *currentFace = new std::vector<Face*>();
    std::vector<Face*> *previousFace = new std::vector<Face*>();

    // Create an icosahedron
    double d = sqrt(pow(((sqrt(5.0) + 1) / 2), 2) + 1); // Mangle the golden ratio to make the radius of the icosahedron = 1
    double l = ((sqrt(5.0) + 1) / 2) / d;
    double s = 1.0 / d;

    currentVertex->push_back(Vector3(-l, -s,  0) * _radius); // 0  Back Left       // Light Green
    currentVertex->push_back(Vector3(-l,  s,  0) * _radius); // 1  Front Left
    currentVertex->push_back(Vector3( l,  s,  0) * _radius); // 2  Front Right
    currentVertex->push_back(Vector3( l, -s,  0) * _radius); // 3  Back Right

    currentVertex->push_back(Vector3( 0, -l, -s) * _radius); // 4  Back Bottom     // Dark Green
    currentVertex->push_back(Vector3( 0, -l,  s) * _radius); // 5  Back Top
    currentVertex->push_back(Vector3( 0,  l,  s) * _radius); // 6  Front Top
    currentVertex->push_back(Vector3( 0,  l, -s) * _radius); // 7  Front Bottom

    currentVertex->push_back(Vector3(-s,  0, -l) * _radius); // 8  Left Bottom     // Purple
    currentVertex->push_back(Vector3(-s,  0,  l) * _radius); // 9  Left Top
    currentVertex->push_back(Vector3( s,  0,  l) * _radius); // 10 Right Top
    currentVertex->push_back(Vector3( s,  0, -l) * _radius); // 11 Right Bottom

    currentFace->push_back(new Face(currentVertex->at(9), currentVertex->at(0), currentVertex->at(1)));
	currentFace->push_back(new Face(currentVertex->at(9), currentVertex->at(1), currentVertex->at(6)));
	currentFace->push_back(new Face(currentVertex->at(9), currentVertex->at(6), currentVertex->at(10)));
	currentFace->push_back(new Face(currentVertex->at(9), currentVertex->at(10), currentVertex->at(5)));
	currentFace->push_back(new Face(currentVertex->at(9), currentVertex->at(5), currentVertex->at(0)));
	
	currentFace->push_back(new Face(currentVertex->at(11), currentVertex->at(7), currentVertex->at(8)));
	currentFace->push_back(new Face(currentVertex->at(11), currentVertex->at(8), currentVertex->at(4)));
	currentFace->push_back(new Face(currentVertex->at(11), currentVertex->at(4), currentVertex->at(3)));
	currentFace->push_back(new Face(currentVertex->at(11), currentVertex->at(3), currentVertex->at(2)));
	currentFace->push_back(new Face(currentVertex->at(11), currentVertex->at(2), currentVertex->at(7)));

    currentFace->push_back(new Face(currentVertex->at(1), currentVertex->at(7), currentVertex->at(6)));
	currentFace->push_back(new Face(currentVertex->at(6), currentVertex->at(7), currentVertex->at(2)));
	currentFace->push_back(new Face(currentVertex->at(6), currentVertex->at(2), currentVertex->at(10)));
	currentFace->push_back(new Face(currentVertex->at(10), currentVertex->at(2), currentVertex->at(3)));
	currentFace->push_back(new Face(currentVertex->at(10), currentVertex->at(3), currentVertex->at(5)));
	
	currentFace->push_back(new Face(currentVertex->at(3), currentVertex->at(4), currentVertex->at(5)));
	currentFace->push_back(new Face(currentVertex->at(5), currentVertex->at(4), currentVertex->at(0)));
	currentFace->push_back(new Face(currentVertex->at(0), currentVertex->at(4), currentVertex->at(8)));
	currentFace->push_back(new Face(currentVertex->at(1), currentVertex->at(0), currentVertex->at(8)));
	currentFace->push_back(new Face(currentVertex->at(1), currentVertex->at(8), currentVertex->at(7)));
    
    // Subdivision of Icosahedron into a sphere
    for (int i = 0; i < _quality; ++i)
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

            Vector3 p01 = ((p0 + p1) / 2).GetNormalized() * _radius;
            Vector3 p12 = ((p1 + p2) / 2).GetNormalized() * _radius;
            Vector3 p02 = ((p0 + p2) / 2).GetNormalized() * _radius;

            currentFace->push_back(new Face(p0, p01, p02));
            currentFace->push_back(new Face(p1, p12, p01));
            currentFace->push_back(new Face(p2, p02, p12));
            currentFace->push_back(new Face(p01, p12, p02));
        }
    }

    // Copy final lists over
    for (int i = 0; i < currentVertex->size(); ++i)
    {
        _vertexList.push_back(currentVertex->at(i));
    }

    for (int i = 0; i < currentFace->size(); ++i)
    {
        _mesh.push_back(currentFace->at(i));
    }

    delete currentFace, currentVertex, previousFace, previousVertex;
}
