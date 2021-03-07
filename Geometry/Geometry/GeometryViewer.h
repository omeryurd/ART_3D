#ifndef _GEOMETRYVIEWER_H
#define _GEOMETRYVIEWER_H

#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "MonolithApp.h"

#include "BoxModule.h"
#include "CrossSectionModule.h"

class GeometryViewer : public MonolithApp
{

public:
    GeometryViewer(bool stereo, bool tracking);
    ~GeometryViewer();

    void KeyboardHandler(unsigned char key, int x, int y);
    void MouseClickHandler(int button, int state, int x, int y);
    void MouseMoveHandler(int x, int y);
    void IdleHandler();
    void DrawScene();
    void MenuHandler(int item);
    void MousePassiveMoveHandler(int x, int y);

    void ChangeModule(std::string moduleName);

//private:

    GeoModule *current;
    BoxModule *boxModule;
    CrossSectionModule *crossModule;
};

#endif
