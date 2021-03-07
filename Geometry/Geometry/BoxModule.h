#ifndef _BoxModule_H
#define _BoxModule_H

#include <Windows.h>
#include <GL/glut.h>

#include <Vector3.h>

#include "GeoModule.h"
#include "PolygonBox.h"
#include "GLMenu.h"

class BoxModule : public GeoModule
{

public:
    BoxModule();
    ~BoxModule();

    std::string GetModuleName();

    void KeyboardHandler(unsigned char key, int x, int y);
    void IdleHandler(MTF::Wand wand, MTF::Camera *camera);
    void Draw();

private:

    PolygonBox *polyBox;

    int highlightIndex;
    std::vector<int> selectedIndex;

    std::string message;

    GLMenuItem *glmiDrawLine;
    GLMenuItem *glmiDrawTriangle;
    GLMenuItem *glmiFindLength;
    GLMenuItem *glmiFindAngle;
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

    void SetupMenu();
    void Reset();
    void ShowMessage();
    bool UpdateWand(MTF::Wand wand, MTF::Camera *camera);
};

#endif
