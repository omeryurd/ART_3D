#ifndef _GLMENU_H
#define _GLMENU_H

#include <vector>
#include <GL/glut.h>

#include <Wand.h>

#include "GLMenuItem.h"

class GLMenu
{

public:
    GLMenu(std::vector<GLMenuItem> menuItems);
    ~GLMenu();

    bool ChangeMenuLocation(MTF::Wand wand);
    bool ChangeMenuLocation(int x, int y);

    bool Update(MTF::Wand wand);
    bool Update(int x, int y);

    bool GetWandPos(MTF::Wand wand, int &x, int &y);

    bool IsMenuItemSelected();
    GLMenuItem* Select();

    void Draw();

private:
    std::vector<GLMenuItem> _menuItems;

    int _centerPosX;
    int _centerPosY;
    int _selectedIndex;

    void *_font;

    int _windowWidth;
    int _windowHeight;

    int _border;
    
    int _menuItemWidth;
    int _menuItemHeight;
    int _menuItemGap;

    int _width;
    int _height;

    void DrawText(std::string str, int x, int y);

    void Intersect(int x, int y);
};

#endif