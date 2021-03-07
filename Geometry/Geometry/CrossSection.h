#ifndef _CROSSSECTION_H
#define _CROSSSECTION_H

#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

#include <Vector3.h>

#include "Face.h"
#include "Plane.h"

class CrossSection
{
public:
    CrossSection(std::vector<Face*> _faces, Plane *_plane, int _size, int _xPos, int _yPos);
    ~CrossSection();

    void SetPlane(Plane *_plane);
    Plane* GetPlane();

    void Render();

    MTF::Vector3 GetCenter();
    void SetCenter(MTF::Vector3 _center);

    double GetPitch();
    void SetPitch(double _pitch);

    double GetYaw();
    void SetYaw(double _yaw);

    double GetRadius();
    void SetRadius(double _radius);

    void CalculateCrossSection();

    void SetFaces(std::vector<Face*> _faces);

private:
    std::vector<Face*> faces;
    Plane *plane;
    
    int size;

    int xPos;
    int yPos;

    GLuint texture;
    GLuint textureFBO;
    GLuint textureDepthBuffer;

    void CreateTexture();

    bool IsPointInPolygon(double x, double y, double *polygonX, double *polygonY, int length);
};

#endif