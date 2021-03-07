#include "Polygon3D.h"

using namespace MTF;


bool TriCompare(const Tri& t1, const Tri& t2)
{
    return t1.z < t2.z;
}


Polygon3D::Polygon3D(Vector3 position, double scale)
{
    _position = position;
    _scale = scale;

    CreateCheckerBoard();

    Reset();
}


Polygon3D::~Polygon3D()
{
}


void Polygon3D::AdjustRotationX(double rotation)
{
    _rotationX += rotation;
}


void Polygon3D::AdjustRotationY(double rotation)
{
    _rotationY += rotation;
}


void Polygon3D::AdjustRotationZ(double rotation)
{
    _rotationZ += rotation;
}


void Polygon3D::AdjustCuttingPlaneRotationX(double rotation)
{
    _cuttingPlaneRotationX += rotation;
}


void Polygon3D::AdjustCuttingPlaneRotationZ(double rotation)
{
    _cuttingPlaneRotationZ += rotation;
}
 

void Polygon3D::AdjustCuttingPlaneHeightOffset(double offset)
{
    _cuttingPlaneHeightOffset += offset;
}
    
 
void Polygon3D::SetScale(double scale)
{
    _scale = scale;
}
 

double Polygon3D::GetScale()
{
    return _scale;
}

 
void Polygon3D::SetTransparency(double transparency)
{
    _transparency = transparency;
}
 

double Polygon3D::GetTransparency()
{
    return _transparency;
}

 
void Polygon3D::Reset()
{
    _rotationX = 0.0;
    _rotationY = 0.0;
    _rotationZ = 0.0;

    _cuttingPlaneRotationX = 0;
    _cuttingPlaneRotationZ = 0;
    _cuttingPlaneHeightOffset = 0;

    _transparency = 1.0;
}


void Polygon3D::DrawString(int x, int y, std::string str)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(1.0, 1.0, 1.0, 1.0);

    void *font = GLUT_BITMAP_TIMES_ROMAN_24;

    int offset = glutBitmapLength(font, (const unsigned char*)str.c_str());

    glRasterPos2i(x - offset / 2, y);
    for (int i = 0; i < str.length(); i++)
        glutBitmapCharacter(font, str.at(i));
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}


void Polygon3D::DrawName()
{
    int width = glutGet(GLUT_WINDOW_WIDTH); 
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    DrawString(width / 2, height - 65, _polygonName);
}


void Polygon3D::DrawCylinder(Vector3 start, Vector3 end, double radius)
{
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    double subdivisions = 13;

    double length = start.GetDistance(end);
    Vector3 oldDir = Vector3(0.0, 0.0, 1.0);
    Vector3 direction = (end - start).GetNormalized();

    double rotationAngle = acos(oldDir.DotProduct(direction)) * 180 / 3.14159;
    Vector3 rotationAxis = oldDir.CrossProduct(direction);

    glPushMatrix();
        glTranslatef(start.GetX(), start.GetY(), start.GetZ());
        glRotatef(rotationAngle, rotationAxis.GetX(), rotationAxis.GetY(), rotationAxis.GetZ());
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluCylinder(quadric, radius, radius, length, subdivisions, 1);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}


void Polygon3D::DrawSphere(Vector3 center, double radius)
{
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    double subdivisions = 15;

    glPushMatrix();
        glTranslatef(center.GetX(), center.GetY(), center.GetZ());
        gluSphere(quadric, radius, subdivisions, subdivisions);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}


bool Polygon3D::IntersectSpheres(Vector3 rayOrigin, Vector3 rayEnd, double radius, int &selectedIndex)
{
    double radiusSquared = radius * radius;

    Vector3 direction = (rayEnd - rayOrigin).GetNormalized();

    double x1 = rayOrigin.GetX();
    double y1 = rayOrigin.GetY();
    double z1 = rayOrigin.GetZ();

    double x2 = rayEnd.GetX();
    double y2 = rayEnd.GetY();
    double z2 = rayEnd.GetZ();

    double x2x1 = x2 - x1;
    double y2y1 = y2 - y1;
    double z2z1 = z2 - z1;

    double a = x2x1 * x2x1 + y2y1 * y2y1 + z2z1 * z2z1;
    double c1 = x1 * x1 + y1 * y1 + z1 * z1;

    selectedIndex = -1;
    bool isSelected = false;
    int length = _vertexList.size();

    Matrix4 pitch, yaw, rot;
    pitch.MakePitchRotationMatrix(_rotationX * 3.14159 / 180);
    yaw.MakeYawRotationMatrix(_rotationY * 3.14159 / 180);
    rot = pitch * yaw;
    
    for (int i = 0; i < length; ++i)
    {
        Vector3 pos = rot * _vertexList[i];
        double x3 = pos.GetX() + _position.GetX();
        double y3 = pos.GetY() + _position.GetY();
        double z3 = pos.GetZ() + _position.GetZ();

        double b = 2 * (x2x1 * (x1 - x3) + y2y1 * (y1 - y3) + z2z1 * (z1 - z3));
        double c = x3 * x3 + y3 * y3 + z3 * z3 + c1 - 2 * (x3 * x1 + y3 * y1 + z3 * z1) - radiusSquared;

        double expr = b * b - 4 * a * c;

        if (expr >= 0)
        {
            isSelected = true;
            if (selectedIndex == -1 || (_vertexList[i].GetDistance(rayOrigin) < _vertexList[selectedIndex].GetDistance(rayOrigin)))
            {
                selectedIndex = i;
            }
        }
    }

    return isSelected;
}


bool Polygon3D::GetMouseRays(int x, int y, MTF::Vector3 &rayOrigin, MTF::Vector3 &rayEnd)
{    
    // Grab the view & projection matrices and the viewport settings
    GLdouble modelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    GLdouble projMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    bool success;
    double pos[3];

    success = gluUnProject(x, viewport[3] - y, 0, modelMatrix, projMatrix, viewport, &pos[0], &pos[1], &pos[2]);
    rayOrigin.Set(pos[0], pos[1], pos[2]);

    success &= gluUnProject(x, viewport[3] - y, 1, modelMatrix, projMatrix, viewport, &pos[0], &pos[1], &pos[2]);
    rayEnd.Set(pos[0], pos[1], pos[2]);

    return success;
}


char Polygon3D::GetLabel(int index)
{
    return (char)('A' + index);
}


Vector3 Polygon3D::GetPosition()
{
    return _position;
}


double Polygon3D::GetRotationX()
{
    return _rotationX;
}


double Polygon3D::GetRotationY()
{
    return _rotationY;
}
 

double Polygon3D::GetRotationZ()
{
    return _rotationZ;
}


void Polygon3D::DrawSort()
{
    int maxSize = _mesh.size() * 3 * 7 + _mesh.size() * 2 + 1; // tri * 3 vertex * 7 data points + triangle * marker
    GLfloat *feedBuffer = new GLfloat[maxSize];
    GLint size;

    glFeedbackBuffer(maxSize, GL_3D_COLOR, feedBuffer);
    glRenderMode(GL_FEEDBACK);
    DrawTriangles();
    size = glRenderMode(GL_RENDER);
    
    int index = 0;
    int pos = 0;
    triangles.clear();

    for (int j = 0; j < _mesh.size(); j++)
    {
        index += 2;
        
        float z = (feedBuffer[index + 2] + feedBuffer[index + 9] + feedBuffer[index + 16]) / 3.0; // Average the triangle's vertex depth
        triangles.push_back(Tri(z, pos));

        ++pos;
        index += 21;
    }

    delete feedBuffer;

    std::sort(triangles.begin(), triangles.end(), TriCompare);

    DrawTriangles(true);
}


void Polygon3D::CreateCheckerBoard()
{
    int numCheckers = 20;

    double size = 20;

    Vector3 corner1(-size, 0, 0);
    Vector3 corner2(-size, 0, -size * 2);
    Vector3 corner3( size, 0, 0);

    Vector3 iInc = (corner2 - corner1) / numCheckers;
    Vector3 jInc = (corner3 - corner1) / numCheckers;

    for (int i = 0; i < numCheckers; ++i)
    {
        for (int j = 0; j < numCheckers; ++j)
        {
            Vector3 v1 = corner1 + iInc * i + jInc * j;
            Vector3 v2 = corner1 + iInc * (i + 1) + jInc * j;
            Vector3 v3 = corner1 + iInc * (i + 1) + jInc * (j + 1);
            Vector3 v4 = corner1 + iInc * i + jInc * (j + 1);

            if (i % 2 == j % 2)
            {
                _checkerBoard.push_back(Quad(v1, v2, v3, v4, 0.0, 0.0, 0.0));
            }
            else
            {
                _checkerBoard.push_back(Quad(v1, v2, v3, v4, 0.5, 0.5, 0.5));
            }
        }
    }
}


void Polygon3D::DrawCheckerBoard()
{
    glBegin(GL_QUADS);
    for (int i = 0; i < _checkerBoard.size(); ++i)
    {
        Quad *q = &_checkerBoard[i];
        glColor4f(q->r, q->g, q->b, 1.0);
        glVertex3f(q->v1.GetX(), q->v1.GetY(), q->v1.GetZ());
        glVertex3f(q->v2.GetX(), q->v2.GetY(), q->v2.GetZ());
        glVertex3f(q->v3.GetX(), q->v3.GetY(), q->v3.GetZ());
        glVertex3f(q->v4.GetX(), q->v4.GetY(), q->v4.GetZ());
    }
    glEnd();
}