#include "GLMenu.h"

using namespace MTF;

GLMenu::GLMenu(std::vector<GLMenuItem> menuItems)
{
    _menuItems = menuItems;

    _selectedIndex = -1;
    _centerPosX = _centerPosY = 0;

    _font = GLUT_BITMAP_9_BY_15;
    _font = GLUT_BITMAP_HELVETICA_18;

    int maxLengthIndex = 0;
    for (int i = 0; i < _menuItems.size(); ++i)
    {
        if (_menuItems[i].name.length() > _menuItems[maxLengthIndex].name.length())
        {
            maxLengthIndex = i;
        }
    }

    _border = 5;
    
    _menuItemHeight = 37;
    _menuItemWidth = _border * 4 + glutBitmapLength(_font, (const unsigned char*)_menuItems[maxLengthIndex].name.c_str()) + _menuItemHeight;
    _menuItemGap = 2;

    _width = _menuItemWidth + _border * 2;
    _height = (_menuItems.size() * _menuItemHeight) + (_menuItems.size() - 1) * _menuItemGap + (_border * 4);
}


GLMenu::~GLMenu(void)
{
}


bool GLMenu::Update(Wand wand)
{
    _selectedIndex = -1;

    _windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    _windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    int x, y;

    if (GetWandPos(wand, x, y))
    {
        return Update(x, y);
    }

    return false;
}


bool GLMenu::Update(int x, int y)
{
    //y = _windowHeight - y;

    int x0 = _centerPosX - (_width / 2);
    int y0 = _centerPosY - (_height / 2);
    int x1 = _centerPosX + (_width / 2);
    int y1 = _centerPosY + (_height / 2);

    bool intersectMenu = x >= x0 && x <= x1 && y >= y0 && y <= y1;

    if (intersectMenu)
    {
        Intersect(x, y);
        return true;
    }

    return false;
}


bool GLMenu::GetWandPos(Wand wand, int &x, int &y)
{
    x = y = -1;

    // Find intersection of wand and full screen quad at screen depth
    Vector3 LL = Vector3(-6.6667, 0.0, 0.0);
    Vector3 LR = Vector3( 6.6667, 0.0, 0.0);
    Vector3 UL = Vector3(-6.6667, 7.5, 0.0);

    double screenWidth = (LR - LL).GetLength();
    double screenHeight = (UL - LL).GetLength();

    Vector3 pos = wand.GetPosition();
    Vector3 view = wand.GetViewVector();

    if (view.GetZ() > 0)
    {
        Vector3 ip = pos + view * fabs(pos.GetZ() / view.GetZ());
        double xPos = ip.GetX() - LL.GetX();
        double yPos = ip.GetY() - LL.GetY();

        if (xPos > 0 && xPos < screenWidth && yPos > 0 && yPos < screenHeight)
        {
            x = (int)((xPos / screenWidth) * _windowWidth + 0.5);
            y = (int)((yPos / screenHeight) * _windowHeight + 0.5);

            return true;
        }
    }

    return false;
}


bool GLMenu::ChangeMenuLocation(Wand wand)
{
    int x, y;
    bool onScreen = GetWandPos(wand, x, y);
    if (onScreen)
    {
        return ChangeMenuLocation(x, y);
    }
    return false;
}


bool GLMenu::ChangeMenuLocation(int x, int y)
{
    // Make sure the full menu stays on the screen
    if (x - _width / 2 < 0)
        x = _width / 2;
    else if (x + _width / 2 > _windowWidth)
        x = _windowWidth - _width / 2;

    if (y - _height / 2 < 0)
        y = _height / 2;
    if (y + _height / 2 > _windowHeight)
        y = _windowHeight - _height / 2;

    _centerPosX = x;
    _centerPosY = y;
    return true;
}


void GLMenu::Draw()
{
    int x0 = _centerPosX - (_width / 2);
    int y0 = _centerPosY - (_height / 2);
    int x1 = _centerPosX + (_width / 2);
    int y1 = _centerPosY + (_height / 2);

    glPushMatrix();
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, _windowWidth, 0.0, _windowHeight);
        glMatrixMode(GL_MODELVIEW);

        glPushMatrix();
            // Draw Menu Background
            glLoadIdentity();

            glColor4f(1.0, 1.0, 1.0, 1.0);
            glBegin(GL_QUADS);
                 glVertex2i(x0, y0);
                 glVertex2i(x0, y1);
                 glVertex2i(x1, y1);
                 glVertex2i(x1, y0);
            glEnd();

            glColor4f(0.0, 0.0, 0.0, 1.0);
            glBegin(GL_QUADS);
                glVertex2i(x0+2, y0+2);
                glVertex2i(x0+2, y1-2);
                glVertex2i(x1-2, y1-2);
                glVertex2i(x1-2, y0+2);
            glEnd();

            
            for (int i = 0; i < _menuItems.size(); ++i)
            {
                // Draw Button Outlines
                int mX0 = x0 + _border;
                int mX1 = x0 + _border + (_menuItemHeight + 1) / 2;
                int mX2 = x0 + _border + _menuItemWidth - (_menuItemHeight + 1) / 2;
                int mX3 = x0 + _border + _menuItemWidth;

                int mY0 = y0 + (_border * 2) + i * (_menuItemHeight + _menuItemGap);
                int mY1 = mY0 + (_menuItemHeight + 1) / 2;
                int mY2 = mY0 + _menuItemHeight;

                glColor4f(1.0, 1.0, 1.0, 1.0);

                glBegin(GL_POLYGON);
                    glVertex2i(mX1, mY0);
                    glVertex2i(mX0, mY1);
                    glVertex2i(mX1, mY2);
                    glVertex2i(mX2, mY2);
                    glVertex2i(mX3, mY1);
                    glVertex2i(mX2, mY0);
                glEnd();

                if (_selectedIndex == i)
                    glColor4f(0.0, 0.0, 0.7, 1.0);
                else
                    glColor4f(0.0, 0.0, 0.0, 1.0);
                    
                glBegin(GL_POLYGON);
                    glVertex2i(mX1+2, mY0+2);
                    glVertex2i(mX0+2, mY1);
                    glVertex2i(mX1+2, mY2-2);
                    glVertex2i(mX2-2, mY2-2);
                    glVertex2i(mX3-2, mY1);
                    glVertex2i(mX2-2, mY0+2);
                glEnd();

                // Draw Button Text
                glColor4f(1.0, 1.0, 1.0, 1.0);

                int tX0 = _centerPosX;
                int tY0 = mY0 + _menuItemHeight / 2 - 6;

                DrawText(_menuItems[_menuItems.size() - i - 1].name, tX0, tY0);
            }

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}


GLMenuItem* GLMenu::Select()
{
    std::cout << "Select: ";

    if (_selectedIndex != -1)
    {
        std::cout << _menuItems[_menuItems.size() - _selectedIndex - 1].name << std::endl;
        return &_menuItems[_menuItems.size() - _selectedIndex - 1];
    }

    std::cout << "NULL" << std::endl;
    return NULL;
}


void GLMenu::DrawText(std::string str, int x, int y)
{
    int offset = glutBitmapLength(_font, (const unsigned char*)str.c_str());
    
    glPushMatrix();
        glRasterPos2i(x - offset / 2, y);
        for (int i = 0; i < str.length(); i++)
            glutBitmapCharacter(_font, str.at(i));
    glPopMatrix();
}


void GLMenu::Intersect(int x, int y)
{
    _selectedIndex = -1;

    int x0 = _centerPosX - (_width / 2);
    int y0 = _centerPosY - (_height / 2);
    int x1 = _centerPosX + (_width / 2);
    int y1 = _centerPosY + (_height / 2);
        
    for (int i = 0; i < _menuItems.size(); ++i)
    {
        // Draw Button Outlines
        int mX0 = x0 + _border;
        int mX1 = x0 + _border + _menuItemWidth;

        int mY0 = y0 + (_border * 2) + i * (_menuItemHeight + _menuItemGap);
        int mY1 = mY0 + _menuItemHeight;

        if (x >= mX0 && x <= mX1 && y >= mY0 && y <= mY1)
        {
            _selectedIndex = i;
            break;
        }
    }
}


bool GLMenu::IsMenuItemSelected()
{
    return _selectedIndex >= 0;
}
