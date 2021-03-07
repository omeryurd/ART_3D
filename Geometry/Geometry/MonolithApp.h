#ifndef _MonolithApp_H
#define _MonolithApp_H

#include <Windows.h>
#include <GL/glut.h>

#include <Monolith.h>

class MonolithApp
{

public:
    MTF::Camera *camera;

    MTF::Monolith *monolith;

    MTF::Display *display;

    bool tracking;

    MonolithApp(bool stereo, bool tracking);
    ~MonolithApp();

    virtual void KeyboardHandler(unsigned char key, int x, int y)=0;
    virtual void MouseClickHandler(int button, int state, int x, int y)=0;
    virtual void MouseMoveHandler(int x, int y)=0;
    virtual void MousePassiveMoveHandler(int x, int y)=0;
    virtual void IdleHandler()=0;
    virtual void DrawScene()=0;

    void Draw();

private:
    bool stereo;

    void DrawMono();
    void DrawQuadStereo();
    void DrawQuadStereoNoHeadTracking();
    void SetupCallback();
    
};

#endif