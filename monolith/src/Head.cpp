#include "Head.h"

namespace MTF
{

    Head::Head()
    {
        _position = Vector3(0.0, 5.0, 5.0);
        _view  = Vector3::UNIT_Z;
        _up    = Vector3::UNIT_Y;
        _right = _view.CrossProduct(_up);
        _tracked = false;
    }


    Head::~Head()
    {
    }

 
    void Head::Update(DTrack_Body_Type_d data)
    {
        _tracked = data.quality != -1;
        if (data.quality > 0) 
        {
            Matrix4 mat(data.rot[0], data.rot[3], data.rot[6], 0.0,
                        data.rot[1], data.rot[4], data.rot[7], 0.0,
                        data.rot[2], data.rot[5], data.rot[8], 0.0,
                                0.0,         0.0,         0.0, 1.0);

            _position = Vector3(data.loc);
            _position *= 3.2808399 / 1000; // mm to foot conversion

            _view  = mat * Vector3::UNIT_Z;
            _up    = mat * Vector3::UNIT_Y;
            _right = mat * Vector3::UNIT_X;
        }
    }


    Vector3 Head::GetPosition()
    {
        return _position;
    }


    Vector3 Head::GetViewVector()
    {
        return _view;
    }


    Vector3 Head::GetUpVector()
    {
        return _up;
    }
 

    Vector3 Head::GetRightVector()
    {
        return _right;
    }


    bool Head::IsTracked()
    {
        return _tracked;
    }


    Matrix4 Head::GetTransformMatrix()
    {
        return Matrix4(_position, _view, _right, _up);
    }


    Head Head::GetCopy(void)
    {
        Head h;
        h._tracked = _tracked;
        h._position = Vector3(_position.GetX(), _position.GetY(), _position.GetZ());
        h._view = Vector3(_view.GetX(), _view.GetY(), _view.GetZ());
        h._up = Vector3(_up.GetX(), _up.GetY(), _up.GetZ());
        h._right = Vector3(_right.GetX(), _right.GetY(), _right.GetZ());

        return h;
    }

}