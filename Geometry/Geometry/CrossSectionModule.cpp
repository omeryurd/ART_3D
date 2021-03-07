#include "CrossSectionModule.h"

using namespace MTF;

extern void ChangeModule(std::string moduleName);


CrossSectionModule::CrossSectionModule(void)
{
    SetupMenu();
    
    Vector3 position = Vector3(0.0, 5.0, -10.0);

    /**/
    polyBox = new PolygonBox(position, 7.0, 5.0, 2.0);    
    polySphere = new PolygonSphere(position, 3.0, 3);
    polyCylinder = new PolygonCylinder(position, 7.0, 3.0, 50);
    polyCone = new PolygonCone(position, 7.0, 3.5, 50);
    /**/

    /*
    polyBox = new PolygonBox(position, 7.0, 5.0, 1.0);    
    polySphere = new PolygonSphere(position, 3.0, 1);
    polyCylinder = new PolygonCylinder(position, 7.0, 3.0, 10);
    polyCone = new PolygonCone(position, 7.0, 3.5, 10);
    /**/
    
    currentPolygon = polyBox;

    cuttingPlane = new Plane(position, 9.0);

    crossSection = new CrossSection(currentPolygon->_mesh, cuttingPlane, 512, 0, 0);

    Reset();
}


CrossSectionModule::~CrossSectionModule(void)
{
}


std::string CrossSectionModule::GetModuleName()
{
    return std::string("Cross-Section Module");
}


void CrossSectionModule::KeyboardHandler(unsigned char key, int x, int y)
{
    // If the ESC key (ASCII code 27 in decimal) is pressed, exit the application
    if (key == 27)
    {
        if (*currentMenuItem == *glmiNONE)
        {
            exit(0);
        }
        else
        {
            currentMenuItem = glmiNONE;
        }
    }
}


void CrossSectionModule::IdleHandler(Wand wand, Camera *camera)
{
    wandPos = wand.GetPosition(camera);
    wandDir = wand.GetViewVector(camera);
    
    //std::cout << wandPos.ToString();

    bool inMenu = false;
    bool highlight = false;
    
    // Update the menu with the wand
    if (showMenu)
    {
        inMenu = menu->Update(wand);
        wandDist = fabs(wandPos.GetZ() / wand.GetViewVector().GetZ());
    }
    // Or update which vertex is highlighted
    else
    {
        highlight = UpdateWand(wand, camera);
        wandDist = 100;
    }

    // Handle object rotation
    double joyH = wand.GetJoystickHorizontal();
    double joyV = wand.GetJoystickVertical();

    if (fabs(joyH) > 0.15)
    {
        if (joyH < 0)
        {
            joyH += 0.15;
            joyH *= 2;
        }
        else
        {
            joyH -= 0.15;
            joyH *= 2;
        }
    }
    else
    {
        joyH = 0;
    }

    if (fabs(joyV) > 0.15)
    {
        if (joyV < 0)
        {
            joyV += 0.15;
            joyV *= -2;
        }
        else
        {
            joyV -= 0.15;
            joyV *= -2;
        }
    }
    else
    {
        joyV = 0;
    }

    // Show menu if right button is pressed
    if (wand.IsRightButtonPressed())
    {
        showMenu = true;
        currentMenuItem = glmiNONE;

        menu->ChangeMenuLocation(wand);
    }
    // If the left button is pressed, close the menu
    else if (wand.isLeftButtonPressed())
    {
        showMenu = false;
    }
    // If the middle button is pressed, close the menu
    else if (wand.isMiddleButtonPressed())
    {
        showMenu = false;
    }
    // If the trigger is pulled
    else if (wand.IsZButtonPressed())
    {
        // This should be adjusted to rotate on the axis defined by the polygon object
        if (joyV != 0 || joyH != 0)
        {
            cuttingPlane->SetPitch(cuttingPlane->GetPitch() + joyV);
            cuttingPlane->SetYaw(cuttingPlane->GetYaw() + joyH);
            crossSection->CalculateCrossSection();
        }

        // If the menu is shown, process the z press as a menu selection
        if (showMenu)
        {
            if (inMenu)
            {
                if (menu->IsMenuItemSelected())
                {
                    currentMenuItem = menu->Select();

                    if (*currentMenuItem == *glmiShapeBox)
                    {
                        currentPolygon = polyBox;
                        currentPolygon->Reset();
                        crossSection->SetFaces(currentPolygon->_mesh);
                    }
                    else if (*currentMenuItem == *glmiShapeSphere)
                    {
                        currentPolygon = polySphere;
                        currentPolygon->Reset();
                        crossSection->SetFaces(currentPolygon->_mesh);
                    }
                    else if (*currentMenuItem == *glmiShapeCone)
                    {
                        currentPolygon = polyCone;
                        currentPolygon->Reset();
                        crossSection->SetFaces(currentPolygon->_mesh);
                    }
                    else if (*currentMenuItem == *glmiShapeCylinder)
                    {
                        currentPolygon = polyCylinder;
                        currentPolygon->Reset();
                        crossSection->SetFaces(currentPolygon->_mesh);
                    }
                    else if (*currentMenuItem == *glmiResetModule)
                    {
                        Reset();
                    }
                    else if (*currentMenuItem == *glmiQuit)
                    {
                        exit(0);
                    }
                    else if (*currentMenuItem == *glmiTransparency100)
                    {
                        currentPolygon->SetTransparency(1.0);
                    }
                    else if (*currentMenuItem == *glmiTransparency75)
                    {
                        currentPolygon->SetTransparency(0.75);
                    }
                    else if (*currentMenuItem == *glmiTransparency50)
                    {
                        currentPolygon->SetTransparency(0.5);
                    }
                    else if (*currentMenuItem == *glmiTransparency25)
                    {
                        currentPolygon->SetTransparency(0.25);
                    }
                    else if (*currentMenuItem == *glmiTransparency0)
                    {
                        currentPolygon->SetTransparency(0.0);
                    }
                    else if (*currentMenuItem == *glmiModuleBox)
                    {
                        // Change to box module
                        message = "";
                        currentMenuItem = glmiNONE;
                        ::ChangeModule("MODULE_BOX");   
                    }
                    else if (*currentMenuItem == *glmiModuleShape)
                    {
                        Reset();
                    }
                    else if (*currentMenuItem == *glmiModuleUnknown)
                    {
                        // Change to unknown module
                        message = "";
                        currentMenuItem = glmiNONE;
                        ::ChangeModule("MODULE_UNKNOWN");
                    }
                }
            }
            showMenu = false;
        }
        // Process the z press as an item selection
        else
        {
            // Nothing to process for this module
        }
    }
    else
    {
        currentPolygon->AdjustRotationY(joyH);
        currentPolygon->AdjustRotationX(joyV);
    }

    glutPostRedisplay();
}


void CrossSectionModule::Draw()
{
    if (showMenu)
    {
        glColor4f(1.0, 0.0, 0.0, 1.0);
        cuttingPlane->Render(currentPolygon->GetRotationX(), currentPolygon->GetRotationY());

        currentPolygon->Draw();

        crossSection->Render();

        ShowMessage();

        menu->Draw();

        glDisable(GL_DEPTH_TEST);
        glColor4f(0.0, 1.0, 0.0, 1.0);
        DrawCylinder(wandPos, wandDir, wandDist, 0.05);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glColor4f(0.0, 1.0, 0.0, 1.0);
        DrawCylinder(wandPos, wandDir, wandDist, 0.05);

        glColor4f(1.0, 0.0, 0.0, 1.0);
        cuttingPlane->Render(currentPolygon->GetRotationX(), currentPolygon->GetRotationY());

        currentPolygon->Draw();

        crossSection->Render();
        
        ShowMessage();
    }
}


void CrossSectionModule::SetupMenu()
{
    glmiShapeBox = new GLMenuItem("Shape: Box");
    glmiShapeSphere = new GLMenuItem("Shape: Sphere");
    glmiShapeCone = new GLMenuItem("Shape: Cone");
    glmiShapeCylinder = new GLMenuItem("Shape: Cylinder");
    glmiTransparency100 = new GLMenuItem("Transparency: 100%");
    glmiTransparency75 = new GLMenuItem("Transparency: 75%");
    glmiTransparency50 = new GLMenuItem("Transparency: 50%");
    glmiTransparency25 = new GLMenuItem("Transparency: 25%");
    glmiTransparency0 = new GLMenuItem("Transparency: 0%");
    glmiModuleBox = new GLMenuItem("Switch to Box Module");
    glmiModuleShape = new GLMenuItem("Switch to Cross-Section Module");
    glmiModuleUnknown = new GLMenuItem("Switch to Unknown Module");
    glmiResetModule = new GLMenuItem("Reset Module");
    glmiQuit = new GLMenuItem("Quit Application");
    glmiNONE = new GLMenuItem("NONE");

    std::vector<GLMenuItem> menuItems;

    menuItems.push_back(*glmiShapeBox);
    menuItems.push_back(*glmiShapeSphere);
    menuItems.push_back(*glmiShapeCone);
    menuItems.push_back(*glmiShapeCylinder);
    menuItems.push_back(*glmiTransparency100);
    menuItems.push_back(*glmiTransparency75);
    menuItems.push_back(*glmiTransparency50);
    menuItems.push_back(*glmiTransparency25);
    menuItems.push_back(*glmiTransparency0);
    menuItems.push_back(*glmiModuleBox);
    menuItems.push_back(*glmiModuleShape);
    menuItems.push_back(*glmiModuleUnknown);
    menuItems.push_back(*glmiResetModule);
    menuItems.push_back(*glmiQuit);

    menu = new GLMenu(menuItems);
}


void CrossSectionModule::Reset()
{
    currentPolygon->Reset();

    cuttingPlane->SetPitch(0.05);
    cuttingPlane->SetYaw(0);

    crossSection->CalculateCrossSection();

    currentMenuItem = glmiNONE;

    message = "";

    showMenu = false;
}


void CrossSectionModule::ShowMessage()
{
    int width = glutGet(GLUT_WINDOW_WIDTH); 
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    currentPolygon->DrawString(width / 2, 60, message);
    currentPolygon->DrawString(width / 2, height - 40, GetModuleName());
}


bool CrossSectionModule::UpdateWand(Wand wand, Camera *camera)
{
    if (!showMenu)
    {
        Vector3 rayOrigin = wand.GetPosition(camera);
        Vector3 rayEnd = rayOrigin + wand.GetViewVector(camera) * 1000;
        //
    }

    return false;
}
