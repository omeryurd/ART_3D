#ifndef _CrossSectionModule_H
#define _CrossSectionModule_H

#include <Windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLU.h>

#include <Vector3.h>

#include "GeoModule.h"

#include "GLMenu.h"
#include "CrossSection.h"
#include "Plane.h"

#include "Polygon3D.h"
#include "PolygonBox.h"
#include "PolygonSphere.h"
#include "PolygonCone.h"
#include "PolygonCylinder.h"

class CrossSectionModule : public GeoModule
{

public:
    CrossSectionModule();
    ~CrossSectionModule();

    std::string GetModuleName();

    void KeyboardHandler(unsigned char key, int x, int y);
    void IdleHandler(MTF::Wand wand, MTF::Camera *camera);
    void Draw();

private:

    Polygon3D *currentPolygon;

    PolygonBox *polyBox;
    PolygonSphere *polySphere;
    PolygonCylinder *polyCylinder;
    PolygonCone *polyCone;

    std::string message;

    GLMenuItem *glmiShapeBox;
    GLMenuItem *glmiShapeSphere;
    GLMenuItem *glmiShapeCone;
    GLMenuItem *glmiShapeCylinder;

    GLMenuItem *glmiTransparency100;
    GLMenuItem *glmiTransparency75;
    GLMenuItem *glmiTransparency50;
    GLMenuItem *glmiTransparency25;
    GLMenuItem *glmiTransparency0;

    GLMenuItem *glmiModuleBox;
    GLMenuItem *glmiModuleShape;
    GLMenuItem *glmiModuleUnknown;

    GLMenuItem *glmiResetModule;
    GLMenuItem *glmiQuit;
    GLMenuItem *glmiNONE;

    GLMenu *menu;

    GLMenuItem *currentMenuItem;

    bool showMenu;

    MTF::Vector3 wandPos;
    MTF::Vector3 wandDir;
    double wandDist;

    CrossSection *crossSection;
    Plane *cuttingPlane;

    void SetupMenu();
    void Reset();
    void ShowMessage();
    bool UpdateWand(MTF::Wand wand, MTF::Camera *camera);

    void MoveCuttingPlane(MTF::Wand wand, MTF::Camera *camera);
};

#endif
