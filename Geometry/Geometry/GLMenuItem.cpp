#include "GLMenuItem.h"


GLMenuItem::GLMenuItem(std::string menuItemName)
{
    name = menuItemName;
}


GLMenuItem::~GLMenuItem()
{
}


bool GLMenuItem::operator == (const GLMenuItem& param)
{
    return (param.name.compare(name) == 0);
}


bool GLMenuItem::operator != (const GLMenuItem& param)
{
    return !(*this == param);
}
