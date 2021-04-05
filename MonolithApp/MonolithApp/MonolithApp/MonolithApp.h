#ifndef _MONOLITHAPP_H
#define _MONOLITHAPP_H

#include <Windows.h>
#include <GL/glut.h>

#include <Monolith.h>

class MonolithApp
{

public:
    // The camera object allows us to control where we are viewing our scene
    MTF::Camera *camera;

    // The monolith object allows us to interact with the tracking devices
    MTF::Monolith *monolith;

    // This object holds the location and dimension of the physical display device
    MTF::Display *display;

    // if tracking is true, then we will try to use wand and head tracking
    bool tracking;

    // Constructor and destructor, do not modify these
    MonolithApp(bool stereo, bool tracking);
    ~MonolithApp();

    // This method is a callback for glutKeyboardFunc
    // * If you wish to capture function keys (f1 - f12) or arrow keys, use glutSpecialFunc
    virtual void KeyboardHandler(unsigned char key, int x, int y)=0;

    // This method is a callback for glutMouseFunc
    virtual void MouseClickHandler(int button, int state, int x, int y)=0;

    // This method is a callback for glutMotionFunc
    // * If you wish to capture mouse movements when no mouse button is pressed, use glutPassiveMotionFunc
    virtual void MouseMoveHandler(int x, int y)=0;

    // This method is a callback for glutIdleFunc
    virtual void IdleHandler()=0;

    // This is where you should draw your scene.  If the program is running in stereo
    // mode then this method may be called multiple times.  The projection and view
    // matricies will be setup before each call to this method.  Because of this, you
    // should not modify data from the draw method, clear any buffers, swap buffers,
    // or change the view or projection matricies.
    virtual void DrawScene()=0;

    // This method is a callback for glutDisplayFunc
    // * You should not modify this method, your drawing code should go in DrawScene
    // This is only public because the way the callbacks are setup require it to be
    void Draw();

private:
    // if stereo is true, then we will use quad buffer stereo rendering
    bool stereo;

    // This code does the setup needed to perform head tracking in a non-stereo window
    // The projection and view matricies are setup here so you should not need to in
    // DrawScene
    void DrawMono();

    // This code does the setup needed to perform head tracking in a stereo window
    // The projection and view matricies are setup here for each eye, so DrawScene
    // will be called twice from this method.
    void DrawQuadStereo();

    // This method registers the glut callbacks so that they call our c++ methods
    void SetupCallback();
    
};

#endif