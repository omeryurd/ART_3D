#include "Demo.h"

using namespace MTF;

Demo::Demo(bool stereo, bool tracking) : MonolithApp(stereo, tracking)
{
    // Display contains the 3 corners of the screen (lower left, lower right,
    // and upper left) in real dimensions (feet) relative to the origin
    display = new Display(Vector3(-6.6667, 0.0, 0.0),
                          Vector3( 6.6667, 0.0, 0.0),
                          Vector3(-6.6667, 7.5, 0.0));

    // Camera used to move around our scene and generate the view/projection matrix
    camera = new Camera(display, 0.1, 100);

    // Create an instance of the framework so we can get tracking information
    monolith = new Monolith(camera, 5000);

    // OpenGL settings, enable lighting and z buffer
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {10.0, 5.0, 10.0, 1.0};

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glDepthFunc(GL_LEQUAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0, 0, 0, 0);
}


Demo::~Demo(void)
{
}


void Demo::DrawScene()
{
    // Draw our array of cubes (10 x 10 x 10)
    DrawCubes();
    
    // Draw a laser pointer
    if (tracking)
    {
        Wand wand = monolith->GetWand();

        // Lighting is disabled so that the cylinder representing the laser does not get shaded
        glDisable(GL_LIGHTING);
        glColor3ub(255, 0, 0);
        DrawCylinder(wand.GetPosition(camera), wand.GetViewVector(camera), 200, 0.05);
        glEnable(GL_LIGHTING);
    }
}


void Demo::KeyboardHandler(unsigned char key, int x, int y)
{
    // If the ESC key (ASCII code 27 in decimal) is pressed, exit the application
    if (key == 27)
        exit(0);
    
    // Simple WASD + Q/E (up/down) movement + R/F (up/down) rotation
    // Shift decreases the speed of tranformations
    if (glutGetModifiers() && GLUT_ACTIVE_SHIFT)
    {
        if (key == 'W' || key == 'w')
            camera->MoveForward(0.05);

        if (key == 'S' || key == 's')
            camera->MoveBackward(0.05);

        if (key == 'A' || key == 'a')
            camera->AdjustYaw(1);

        if (key == 'D' || key == 'd')
            camera->AdjustYaw(-1);

        if (key == 'E' || key == 'e')
            camera->MoveDown(0.05);

        if (key == 'Q' || key == 'q')
            camera->MoveUp(0.05);

        if (key == 'r' || key == 'R')
            camera->AdjustPitch(1);

        if (key == 'f' || key == 'F')
            camera->AdjustPitch(-1);
    }
    else
    {
        if (key == 'W' || key == 'w')
            camera->MoveForward(1.0);

        if (key == 'S' || key == 's')
            camera->MoveBackward(1.0);

        if (key == 'A' || key == 'a')
            camera->AdjustYaw(5);

        if (key == 'D' || key == 'd')
            camera->AdjustYaw(-5);

        if (key == 'E' || key == 'e')
            camera->MoveDown(1);

        if (key == 'Q' || key == 'q')
            camera->MoveUp(1);

        if (key == 'r' || key == 'R')
            camera->AdjustPitch(5);

        if (key == 'f' || key == 'F')
            camera->AdjustPitch(-5);
    }

    // Reset our position and rotation
    if (key == 't' || key == 'T')
    {
        camera->SetPosition(Vector3::ZERO);
        camera->SetPitch(0);
        camera->SetYaw(0);
    }
}


void Demo::MouseClickHandler(int button, int state, int x, int y)
{
    // Handle mouse clicks here if you want
}


void Demo::MouseMoveHandler(int x, int y)
{
    // Handle mouse drag events here if you want
}


void Demo::IdleHandler()
{
    if (tracking)
    {
        // Grab information from the wand, such as its position, direction,
        // button information, and joystick information
        double speed = 0.1;
        Wand wand = monolith->GetWand();

        // We want the position and direction of the wand in object coordinates,
        // so we pass in the camera object, so that the wand can undo the view
        // projection to give us what we need in object space
        Vector3 wandPos = wand.GetPosition(camera);
        Vector3 wandDir = wand.GetViewVector(camera);        

        double joyH = wand.GetJoystickHorizontal();
        double joyV = wand.GetJoystickVertical();

        // If the z button is pressed, move forward in direction wand is pointing
        if (wand.IsButtonPressed(0))
        {
            camera->SetPosition(camera->GetPosition() + wandDir * speed);
        }

        // If the right button is pressed, move backward from direction wand is pointing
        if (wand.IsButtonPressed(1))
        {
            camera->SetPosition(camera->GetPosition() + wandDir * -speed);
        }

        // If the joystick is moved more than 15% from its neutral postion
        // horizontally, rotate left or right as appropriate
        if (fabs(joyH) > 0.15)
        {
            if (joyH > 0)
                joyH -= 0.15;
            else
                joyH += 0.15;

            camera->AdjustYaw(-joyH);
        }

        // If the joystick is moved more than 15% from its neutral postion
        // vertically, rotate up or down as appropriate
        if (fabs(joyV) > 0.15)
        {
            if (joyV > 0)
                joyV -= 0.15;
            else
                joyV += 0.15;

            camera->AdjustPitch(joyV);
        }
    }

    // Tell GLUT that we want to redraw our window
    glutPostRedisplay();
}


// This is only used to render a 10x10x10 array of cubes and may be removed if not needed
void Demo::DrawCubes()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 10; k++)
            {
				glPushMatrix();
                    glColor3ub(0, 255 - k*25, 0);
                    glTranslatef(-25 + 5*i, -25 + 5*j, -5.0 - 5*k);
                    cube.draw_cube();
                glPopMatrix();
            }
        }
    }
}


// This is only used for rendering the laser, and may be removed if it is not needed
void Demo::DrawCylinder(Vector3 position, Vector3 view, double length, double radius)
{
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    double subdivisions = 10;

    //double length = start.GetDistance(end);
    Vector3 start = position;
    Vector3 end = position + view * length;
    Vector3 oldDir = Vector3(0.0, 0.0, 1.0);
    Vector3 direction = (end - start).GetNormalized();

    double rotationAngle = acos(oldDir.DotProduct(direction)) * 180 / 3.14159;
    Vector3 rotationAxis = oldDir.CrossProduct(direction);

    glPushMatrix();
        glTranslatef(start.GetX(), start.GetY(), start.GetZ());
        glRotatef(rotationAngle, rotationAxis.GetX(), rotationAxis.GetY(), rotationAxis.GetZ());
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluCylinder(quadric, radius, radius, length, subdivisions, 1);

        //draw the first cap
        gluQuadricOrientation(quadric,GLU_INSIDE);
        gluDisk( quadric, 0.0, radius, subdivisions, 1);
        glTranslatef(0, 0, length);

        //draw the second cap
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluDisk(quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();

    gluDeleteQuadric(quadric);
}
