#include "MonolithApp.h"

using namespace MTF;

MonolithApp::MonolithApp(bool stereo, bool tracking)
{
    this->stereo = stereo;
    this->tracking = tracking;
    SetupCallback();
}


MonolithApp::~MonolithApp(void)
{
    delete camera;
    delete monolith;
    delete display;
}


void MonolithApp::Draw()
{
    if (stereo)
        DrawQuadStereo();
    else
        DrawMono();
}


void MonolithApp::DrawMono()
{
    double matrix[16];

    if (tracking)
    {
        TrackingBody *head = (TrackingBody*)&monolith->GetHead();

        glMatrixMode(GL_PROJECTION);
        camera->GetProjectionMatrix(Eye::MONO, head).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);

        glMatrixMode(GL_MODELVIEW); 
        camera->GetViewMatrix(Eye::MONO, head).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        camera->GetProjectionMatrix(Eye::MONO).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);

        glMatrixMode(GL_MODELVIEW); 
        camera->GetViewMatrix(Eye::MONO).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawScene();
    
    glutSwapBuffers();
}


void MonolithApp::DrawQuadStereo()
{
    double matrix[16];

    if (tracking)
    {
        TrackingBody *head = (TrackingBody*)&monolith->GetHead();

        glMatrixMode(GL_PROJECTION);
        camera->GetProjectionMatrix(Eye::LEFT, head).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);
    
        glMatrixMode(GL_MODELVIEW); 
        glPushMatrix();
            glDrawBuffer(GL_BACK_LEFT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera->GetViewMatrix(Eye::LEFT, head).GetMatrixArray(matrix);
            glMultMatrixd(matrix);
            DrawScene();
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        camera->GetProjectionMatrix(Eye::RIGHT, head).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glDrawBuffer(GL_BACK_RIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera->GetViewMatrix(Eye::RIGHT, head).GetMatrixArray(matrix);
            glMultMatrixd(matrix);
            DrawScene();
        glPopMatrix();
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        camera->GetProjectionMatrix(Eye::LEFT).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);
    
        glMatrixMode(GL_MODELVIEW); 
        glPushMatrix();
            glDrawBuffer(GL_BACK_LEFT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera->GetViewMatrix(Eye::LEFT).GetMatrixArray(matrix);
            glMultMatrixd(matrix);
            DrawScene();
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        camera->GetProjectionMatrix(Eye::RIGHT).GetMatrixArray(matrix);
        glLoadMatrixd(matrix);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glDrawBuffer(GL_BACK_RIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera->GetViewMatrix(Eye::RIGHT).GetMatrixArray(matrix);
            glMultMatrixd(matrix);
            DrawScene();
        glPopMatrix();
    }

    glutSwapBuffers();
}


// Callback registration for glut
// This allows the use of C++ methods to be registered as a callback function.
// This is required to make the correct links between C (GLUT) and C++ (our app).
// A global variable, currentInstance, is used to store a pointer to our class.
// The "C" callback methods are also global and provide a wrapper to our 
// callback functions in our class.
//
// If you need to use callbacks that are not provided, you can follow the
// pattern used below for the other five GLUT callbacks to add your own.
MonolithApp* currentInstance;

extern "C"
{
    void DrawCallback()
    {
        currentInstance->Draw();
    }

    void KeyboardCallback(unsigned char key, int x, int y)
    {
        currentInstance->KeyboardHandler(key, x, y);
    }

    void MouseClickCallback(int button, int state, int x, int y)
    {
        currentInstance->MouseClickHandler(button, state, x, y);
    }

    void MouseMoveCallback(int x, int y)
    {
        currentInstance->MouseMoveHandler(x, y);
    }

    void IdleCallback()
    {
        currentInstance->IdleHandler();
    }
}

void MonolithApp::SetupCallback()
{
    ::currentInstance = this;
    ::glutDisplayFunc(::DrawCallback);
    ::glutKeyboardFunc(::KeyboardCallback);
    ::glutMouseFunc(::MouseClickCallback);
    ::glutMotionFunc(::MouseMoveCallback);
    ::glutIdleFunc(::IdleCallback);
}
