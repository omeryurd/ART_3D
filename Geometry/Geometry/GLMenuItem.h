#ifndef _GLMENUITEM_H
#define _GLMENUITEM_H

#include <string>
#include <iostream>

class GLMenuItem
{

public:
    GLMenuItem(std::string menuItemName);
    ~GLMenuItem();

    bool operator == (const GLMenuItem&);
    bool operator != (const GLMenuItem&);

    std::string name;
};

#endif