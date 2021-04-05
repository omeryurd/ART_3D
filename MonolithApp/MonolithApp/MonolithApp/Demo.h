#ifndef _DEMO_H
#define _DEMO_H

#include "Cube.h"
#include "MonolithApp.h"

class Demo : public MonolithApp
{
public:
    // Constructor and destructor, make sure to call the parent's constructor
    // if you modify these methods
    Demo(bool stereo, bool tracking);
    ~Demo(void);

    // This method allows you to process a key press, it also tells you the
    // x and y position of the mouse at the time of the key press
    void KeyboardHandler(unsigned char key, int x, int y);

    // This method handles mouse button clicks, it provides which button was
    // involved, the state (mouse click or release), and the position at the
    // time of the event
    void MouseClickHandler(int button, int state, int x, int y);

    // This method handles the position of a mouse as it moves around the
    // window, if one of the mouse buttons is pressed as it moves (dragging)
    void MouseMoveHandler(int x, int y);

    // This method is called whenever window events are not being processed.
    // This should be used to perform updates to the scene if they are needed.
    // GlutPostRedisplay should probably be used here, so that Glut knows to
    // redraw the scene.
    void IdleHandler();

    // This is where your OpenGL draw code should go.  This may be called once
    // or twice per window redraw, depending on the rendermode.  Because of this,
    // you should not modify data in this method, swap buffers, clear buffers,
    // or change the view/projection matricies.
    void DrawScene();

private:
    // Helper class used to draw a cube, this is not needed for your application
    Cube cube;

    // This is a helper method to draw an array of cubes 10 by 10 by 10 in dimension
    // This method is only included to show something on the screen for this demo
    void DrawCubes();

    // This is a helper method to draw a cylinder starting at position and extending
    // in the direction of the view vector for a given length and radius
    void DrawCylinder(MTF::Vector3 position, MTF::Vector3 view, double length, double radius);
};

#endif