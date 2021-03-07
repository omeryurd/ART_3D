#include "Display.h"

namespace MTF
{

    Display::Display(Vector3 lowerLeftCorner, Vector3 lowerRightCorner, Vector3 upperLeftCorner)
    {
        _LL = lowerLeftCorner;
        _LR = lowerRightCorner;
        _UL = upperLeftCorner;

        _width = _LR.GetDistance(_LL);
        _height = _UL.GetDistance(_LL);

	    Vector3 Xs = (_LR - _LL) / _width;
	    Vector3 Ys = (_UL - _LL) / _height;
	    Vector3 Zs = Xs.CrossProduct(Ys);
    
        _MwMatrix = Matrix4(Xs.GetX(), Ys.GetX(), Zs.GetX(),      0.0,
                           Xs.GetY(), Ys.GetY(), Zs.GetY(),      0.0,
                           Xs.GetZ(), Ys.GetZ(), Zs.GetZ(),      0.0,
                                 0.0,       0.0,       0.0,      1.0);
    }


    Display::~Display()
    {
    }


    Vector3 Display::GetScreenUpVector()
    {
        return (_UL - _LL).GetNormalized();
    }


    Vector3 Display::GetScreenRightVector()
    {
        return (_LR - _LL).GetNormalized();
    }


    Vector3 Display::GetScreenOutVector()
    {
        return ((_LR - _LL).CrossProduct(_UL - _LL)).GetNormalized();
    }


    Vector3 Display::GetLowerLeftCorner()
    {
        return _LL;
    }


    Vector3 Display::GetLowerRightCorner()
    {
        return _LR;
    }


    Vector3 Display::GetUpperLeftCorner()
    {
        return _UL;
    }


    Vector3 Display::GetUpperRightCorner()
    {
        return (_LR - _LL) + _UL;
    }


    double Display::GetScreenWidth()
    {
        return _width;
    }


    double Display::GetScreenHeight()
    {
        return _height;
    }
    
    
    Matrix4 Display::GetMwMatrix()
    {
        return _MwMatrix;
    }

}
