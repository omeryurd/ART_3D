#ifndef _GEOMODULE_H
#define _GEOMODULE_H

#include <Windows.h>
#include <GL/glut.h>
#include <string>

#include <Wand.h>
#include <Vector3.h>


class GeoModule
{

public:
    virtual std::string GetModuleName()=0;

    virtual void KeyboardHandler(unsigned char key, int x, int y)=0;
    virtual void IdleHandler(MTF::Wand wand, MTF::Camera *camera)=0;
    virtual void Draw()=0;

    bool GetWandPos(MTF::Wand wand, int &x, int &y, int windowWidth, int windowHeight);

    void DrawCylinder(MTF::Vector3 position, MTF::Vector3 view, double length, double radius);
};

#endif