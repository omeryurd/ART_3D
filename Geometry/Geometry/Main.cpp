// Using this to check for any memory leaks in a debug build
// You must install Visual Leak Detector if you want to use this
//#include <vld.h>

//#define OVERRIDE_NEW_DELETE
//#include "MemProCpp\MemPro.cpp"

#ifdef _WIN32
#  include <direct.h>
#  define getcwd _getcwd
#  define chdir _chrdir
#else
#  include <unistd.h>
#endif

#define GL_GLEXT_PROTOTYPES

#include "GeometryViewer.h"

// If you wish for quad buffer stereo, set the below line to true
#define STEREO true
 
// If you wish for full screen mode, set the below line to true
#define FULLSCREEN false

// If you wish to use wand and head tracking, set the below line to true
#define TRACKING true

// Window size when not in full screen mode
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

GeometryViewer *geoViewer;

int main(int argc, char **argv)
{
    char *dir = new char[256];
    getcwd(dir, 256);
    std::cout << "Current Working Directory: " << dir << std::endl;
    delete dir;

    glutInit(&argc, argv);

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

    if (STEREO)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STEREO);
    else
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutCreateWindow("Geometry Viewer");

    // Run GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(-1);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Check if the GLEW extension we need is present
    if (!GLEW_EXT_framebuffer_object)
    {
        std::cout << "EXT_framebuffer_object is not loaded, it may not be supported by your card." << std::endl;
        exit(-1);
    }
    std::cout << "EXT_framebuffer_object extension is available!" << std::endl;

    if (FULLSCREEN)
        glutFullScreen();

    geoViewer = new GeometryViewer(STEREO, TRACKING);

    glutMainLoop();

    delete geoViewer;

    return 0;
}
