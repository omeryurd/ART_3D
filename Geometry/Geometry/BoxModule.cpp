#include "BoxModule.h"

using namespace MTF;

extern void ChangeModule(std::string moduleName);


BoxModule::BoxModule(void)
{
    SetupMenu();

    //polyBox = new PolygonBox(Vector3(0.0, 0.0, 1.0), 3.0, 2.0, 1.0);
    polyBox = new PolygonBox(Vector3(0.0, 3.8, -3.0), 8.0, 4.0, 2.0);

    Reset();
}


BoxModule::~BoxModule(void)
{
}


std::string BoxModule::GetModuleName()
{
    return std::string("Pythagorean Box Module");
}


void BoxModule::KeyboardHandler(unsigned char key, int x, int y)
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
            selectedIndex.clear();
        }
    }
}


void BoxModule::IdleHandler(Wand wand, Camera *camera)
{
    wandPos = wand.GetPosition(camera);
    wandDir = wand.GetViewVector(camera);

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
        wandDist = 50;
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

    polyBox->AdjustRotationY(joyH);
    polyBox->AdjustRotationX(joyV);

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
        // If the menu is shown, process the z press as a menu selection
        if (showMenu)
        {
            if (inMenu)
            {
                if (menu->IsMenuItemSelected())
                {
                    currentMenuItem = menu->Select();

                    if (*currentMenuItem == *glmiResetModule)
                    {
                        Reset();
                    }
                    else if (*currentMenuItem == *glmiQuit)
                    {
                        exit(0);
                    }
                    else if (*currentMenuItem == *glmiDrawLine)
                    {
                        selectedIndex.clear();
                        message = "Select two points to draw a line between";
                    }
                    else if (*currentMenuItem == *glmiDrawTriangle)
                    {
                        selectedIndex.clear();
                        message = "Select three points for the corners of the triangle";
                    }
                    else if (*currentMenuItem == *glmiFindLength)
                    {
                        selectedIndex.clear();
                        message = "Select two points find the length between them";
                    }
                    else if (*currentMenuItem == *glmiFindAngle)
                    {
                        selectedIndex.clear();
                        message = "Select three points to see the angle formed by them";
                    }
                    else if (*currentMenuItem == *glmiTransparency100)
                    {
                        polyBox->SetTransparency(1.0);
                    }
                    else if (*currentMenuItem == *glmiTransparency75)
                    {
                        polyBox->SetTransparency(0.75);
                    }
                    else if (*currentMenuItem == *glmiTransparency50)
                    {
                        polyBox->SetTransparency(0.5);
                    }
                    else if (*currentMenuItem == *glmiTransparency25)
                    {
                        polyBox->SetTransparency(0.25);
                    }
                    else if (*currentMenuItem == *glmiTransparency0)
                    {
                        polyBox->SetTransparency(0.0);
                    }
                    else if (*currentMenuItem == *glmiModuleBox)
                    {
                        Reset();
                    }
                    else if (*currentMenuItem == *glmiModuleShape)
                    {
                        // Change to shapes module
                        message = "";
                        currentMenuItem = glmiNONE;
                        ::ChangeModule("MODULE_SHAPES");
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
        else if (highlight)
        {
            if (*currentMenuItem == *glmiDrawLine ||
                *currentMenuItem == *glmiDrawTriangle ||
                *currentMenuItem == *glmiFindLength ||
                *currentMenuItem == *glmiFindAngle)
            {
                // Make sure the new vertex hasn't been added already
                bool found = false;
                for (int i = 0; i < selectedIndex.size(); ++i)
                {
                    if (selectedIndex[i] == highlightIndex)
                        found = true;
                }

                if (!found)
                {
                    selectedIndex.push_back(highlightIndex);

                    if (*currentMenuItem == *glmiDrawLine)
                    {
                        if (selectedIndex.size() == 2)
                        {
                            std::stringstream str;
                            str << "Line drawn between points " << polyBox->GetLabel(selectedIndex[0]) << " and " << polyBox->GetLabel(selectedIndex[1]);
                            polyBox->AddEdge(selectedIndex[0], selectedIndex[1]);
                            message = str.str();
                            currentMenuItem = glmiNONE;
                        }
                    }
                    else if (*currentMenuItem == *glmiDrawTriangle)
                    {
                        if (selectedIndex.size() == 3)
                        {
                            std::stringstream str;
                            str << "Triangle drawn with corners " << polyBox->GetLabel(selectedIndex[0]) << ", " << polyBox->GetLabel(selectedIndex[1]) << ", and " << polyBox->GetLabel(selectedIndex[2]);
                            polyBox->AddTriangle(selectedIndex[0], selectedIndex[1], selectedIndex[2]);
                            message = str.str();
                            currentMenuItem = glmiNONE;
                        }
                    }
                    else if (*currentMenuItem == *glmiFindAngle)
                    {
                        if (selectedIndex.size() == 3)
                        {
                            Vector3 v0 = polyBox->_vertexList[selectedIndex[0]] - polyBox->_vertexList[selectedIndex[1]];
                            Vector3 v2 = polyBox->_vertexList[selectedIndex[2]] - polyBox->_vertexList[selectedIndex[1]];
                            double angle = acos(v0.DotProduct(v2) / (v0.GetLength() * v2.GetLength())) * 180 / 3.14159;
                            std::stringstream str;
                            str.precision(3);
                            str << std::fixed;
                            str << "The angle formed by " << polyBox->GetLabel(selectedIndex[0]) << polyBox->GetLabel(selectedIndex[1]) << polyBox->GetLabel(selectedIndex[2]) << " is " << angle << " degrees";
                            message = str.str();
                            currentMenuItem = glmiNONE;
                        }
                    }
                    else if (*currentMenuItem == *glmiFindLength)
                    {
                        if (selectedIndex.size() == 2)
                        {
                            double length = polyBox->_vertexList[selectedIndex[0]].GetDistance(polyBox->_vertexList[selectedIndex[1]]);
                            std::stringstream str;
                            str.precision(3);
                            str << std::fixed;
                            str << "The length of line " << polyBox->GetLabel(selectedIndex[0]) << polyBox->GetLabel(selectedIndex[1]) << " is " << length << " units";
                            message = str.str();
                            currentMenuItem = glmiNONE;
                        }
                    }
                }
            }
        }
    }

    glutPostRedisplay();
}


void BoxModule::Draw()
{
    if (showMenu)
    {
        polyBox->Draw(highlightIndex, selectedIndex);

        ShowMessage();

        menu->Draw();

        glColor3f(0.0, 1.0, 0.0);
        DrawCylinder(wandPos, wandDir, wandDist, 0.05);
    }
    else
    {
        glColor3f(0.0, 1.0, 0.0);
        DrawCylinder(wandPos, wandDir, wandDist, 0.05);

        polyBox->Draw(highlightIndex, selectedIndex);
        
        ShowMessage();
    }
}


void BoxModule::SetupMenu()
{
    glmiDrawLine = new GLMenuItem("Draw Line");
    glmiDrawTriangle = new GLMenuItem("Draw Triangle");
    glmiFindLength = new GLMenuItem("Find Length");
    glmiFindAngle = new GLMenuItem("Find Angle");
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

    menuItems.push_back(*glmiDrawLine);
    menuItems.push_back(*glmiDrawTriangle);
    menuItems.push_back(*glmiFindLength);
    menuItems.push_back(*glmiFindAngle);
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


void BoxModule::Reset()
{
    polyBox->Reset();
    polyBox->Clear();

    currentMenuItem = glmiNONE;

    message = "";

    highlightIndex = -1;
    selectedIndex.clear();

    showMenu = false;
}


void BoxModule::ShowMessage()
{
    int width = glutGet(GLUT_WINDOW_WIDTH); 
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    polyBox->DrawString(width / 2, 60, message);
    polyBox->DrawString(width / 2, height - 40, GetModuleName());
}


bool BoxModule::UpdateWand(Wand wand, Camera *camera)
{
    if (!showMenu)
    {
        Vector3 rayOrigin = wand.GetPosition(camera);
        Vector3 rayEnd = rayOrigin + wand.GetViewVector(camera) * 1000;
        return polyBox->IntersectSpheres(rayOrigin, rayEnd, polyBox->GetRadius(), highlightIndex);
    }

    return false;
}
