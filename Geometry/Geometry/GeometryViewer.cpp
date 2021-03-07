#include "GeometryViewer.h"

using namespace MTF;

GLfloat lightPos[] = {0.0f, 5.0f, -10.0f, 1.0f};

GeometryViewer::GeometryViewer(bool stereo, bool tracking) : MonolithApp(stereo, tracking)
{
    boxModule = new BoxModule();
    crossModule = new CrossSectionModule();

    //current = boxModule;
    current = crossModule;

    display = new Display(Vector3(-6.6667, 0.0, 0.0),
                          Vector3( 6.6667, 0.0, 0.0),
                          Vector3(-6.6667, 7.5, 0.0));

    camera = new Camera(display, 0.1, 500);

    //camera->SetIODMultiple(0.45);

    monolith = new Monolith(camera, 5000, 20);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_FRONT_AND_BACK);
    glEnable(GL_BLEND);

    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    //glEnable(GL_POLYGON_SMOOTH);

    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0);

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f , 1.0f};

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    //glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
}


GeometryViewer::~GeometryViewer(void)
{
    delete boxModule;
    delete crossModule;
}


void GeometryViewer::KeyboardHandler(unsigned char key, int x, int y)
{
    current->KeyboardHandler(key, x, y);
}


void GeometryViewer::MouseClickHandler(int button, int state, int x, int y)
{   
}


void GeometryViewer::MouseMoveHandler(int x, int y)
{   
}


void GeometryViewer::IdleHandler()
{
    current->IdleHandler(monolith->GetWand(), monolith->GetCamera());
}


void GeometryViewer::MousePassiveMoveHandler(int x, int y)
{
}


void GeometryViewer::DrawScene()
{
    //glEnable(GL_LIGHTING);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    current->Draw();
}


extern GeometryViewer *geoViewer;

void ChangeModule(std::string moduleName)
{
    if (moduleName.compare("MODULE_BOX") == 0)
    {
        geoViewer->current = geoViewer->boxModule;
    }
    else if (moduleName.compare("MODULE_SHAPES") == 0)
    {
        geoViewer->current = geoViewer->crossModule;
    }
    else if (moduleName.compare("MODULE_UNKNOWN") == 0)
    {

    }
}