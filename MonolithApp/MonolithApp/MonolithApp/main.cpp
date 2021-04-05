// This project shows how to create a simple demo that shows an array of cubes
// on the screen with head and wand tracking.  You may turn stereo and fullscreen
// on and off using the booleans in the main method.  This project demonstrates
// how to incorporate head tracking and simple wand interactions into a project.
//
// This project uses GLUT.  For more information on the GLUT API, see:
// http://www.opengl.org/documentation/specs/glut/spec3/spec3.html

// Using this to check for any memory leaks in a debug build
// You must install Visual Leak Detector if you want to use this
//#include <vld.h>

#include "Demo.h"

// If you wish for quad buffer stereo, set the below line to true
#define STEREO true

// If you wish for full screen mode, set the below line to true
#define FULLSCREEN true

// If you wish to use wand and head tracking, set the below line to true
#define TRACKING true

// Window size when not in full screen mode
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


// Entry point to our program
int main(int argc, char **argv)
{
    // Standard glut init, feel free to change initial window size or position
    glutInit(&argc, argv);

    // For full screen mode, we create a window the size of the screen instead of
    // using glutFullScreen(), this makes it easier to alt+tab to other windows
    if (FULLSCREEN)
    {
        glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
        glutInitWindowPosition(0, 0);
    }
    else
    {
        glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        glutInitWindowPosition(0, 20);
    }

    // GLUT_STEREO indicates that we want to do quad buffer stereoscopic rendering
    // This requires a NVIDIA QUADRO graphics card with either a clone mode display
    // or a screen that supports 120 Hz rendering
    // GLUT will throw an error to the console if a pixel mode supporting stereo
    // cannot be found, so make sure to check that if your stereo app is not running
    if (STEREO)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STEREO);
    else
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutCreateWindow("MonolithGlutApp - Cube Demo");

    if (FULLSCREEN)
        glutFullScreen();

    // Create an instance of our application (it registers the glut callbacks)
    Demo *app = new Demo(STEREO, TRACKING);

    glutMainLoop();

    delete app;

    return 0;
}
