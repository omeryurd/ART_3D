#include "Wand.h"

namespace MTF
{

    Wand::Wand()
    {
        _position = Vector3(0.0, 5.0, -5.0);
        _view  = Vector3::NEGATIVE_UNIT_Z;
        _up    = Vector3::UNIT_Y;

        _right = _view.CrossProduct(_up);

        _numButtons = 4;
        _joystickHorizontal = _joystickVertical = 0;

        _rollingAverage = 0;

        _tracked = false;

        for (int i = 0; i < 16; ++i)
            _buttons[i] = false;
    }


    Wand::Wand(int rollingAverage)
    {
        _position = Vector3(0.0, 5.0, -5.0);
        _view  = Vector3::NEGATIVE_UNIT_Z;
        _up    = Vector3::UNIT_Y;

        _right = _view.CrossProduct(_up);

        _numButtons = 4;
        _joystickHorizontal = _joystickVertical = 0;

        _rollingAverage = rollingAverage;

        _tracked = false;

        for (int i = 0; i < 16; ++i)
            _buttons[i] = false;
    }


    Wand::~Wand()
    {
    }


    void Wand::Update(DTrack_FlyStick_Type_d data)
    {
        _tracked = data.quality != -1;

        if (data.quality > 0) 
        {
            Matrix4 mat(data.rot[0], data.rot[3], data.rot[6], 0.0,
                data.rot[1], data.rot[4], data.rot[7], 0.0,
                data.rot[2], data.rot[5], data.rot[8], 0.0,
                0.0,         0.0,         0.0, 1.0);

            _position = Vector3(data.loc);
            // mm to foot conversion  ~3.28 ft to 1 m, 1 m to 1000 mm
            _position *= 3.2808399 / 1000; 
            // We want to give position in eye coordinates, so the wand position should be 
            // measued as distance into the scene as opposed to distance from the screen
            _position *= Vector3(1.0, 1.0, -1.0);

            // Similarly, the view vector should point by default into the negative z axis
            _view  = mat * Vector3::UNIT_Z;
            _up    = mat * Vector3::UNIT_Y;
            _right = mat * Vector3::UNIT_X;

            _view  *= Vector3(-1.0, -1.0, 1.0);
            _up    *= Vector3(-1.0, -1.0, 1.0);
            _right *= Vector3(-1.0, -1.0, 1.0);
        }

        _numButtons = data.num_button;

        for (int i = 0; i < _numButtons; i++)
            _buttons[i] = (data.button[i] == 1);

        _joystickHorizontal = data.joystick[0];
        _joystickVertical = data.joystick[1];

        if (_rollingAverage > 0)
        {
            _previousPositions.push_back(_position);
            if (_previousPositions.size() > _rollingAverage)
            {
                _previousPositions.pop_front();
            }

            _previousViews.push_back(_view);
            if (_previousViews.size() > _rollingAverage)
            {
                _previousViews.pop_front();
            }
        }
    }


    Vector3 Wand::GetPosition()
    {
        if (_rollingAverage > 0 && _previousPositions.size() > 1)
        {
            Vector3 p = Vector3::ZERO;

            int count = 0;
            for(std::list<Vector3>::iterator list_iter = _previousPositions.begin(); list_iter != _previousPositions.end(); ++list_iter)
            {
                p += *list_iter;
                ++count;
            }
            p = p * (1.0 / count);
            return p;
        }

        return _position;
    }


    Vector3 Wand::GetViewVector()
    {
        if (_rollingAverage > 0 && _previousViews.size() > 1)
        {
            Vector3 v = Vector3::ZERO;

            int count = 0;
            for(std::list<Vector3>::iterator list_iter = _previousViews.begin(); list_iter != _previousViews.end(); ++list_iter)
            {
                v += *list_iter;
                ++count;
            }
            v = v * (1.0 / count);
            return v;
        }

        return _view;
    }


    Vector3 Wand::GetUpVector()
    {
        return _up;
    }


    Vector3 Wand::GetRightVector()
    {
        return _right;
    }


    Matrix4 Wand::GetTransformMatrix()
    {
        return Matrix4(_position, _view, _right, _up);
    }


    Vector3 Wand::GetViewVector(Camera *camera)
    {
        Matrix4 cameraRotation = Matrix4(camera->GetViewVector(), camera->GetRightVector(), camera->GetUpVector());
        return cameraRotation * GetViewVector();
    }


    Vector3 Wand::GetRightVector(Camera *camera)
    {
        Matrix4 cameraRotation = Matrix4(camera->GetViewVector(), camera->GetRightVector(), camera->GetUpVector());
        return cameraRotation * _right;
    }


    Vector3 Wand::GetPosition(Camera *camera)
    {
        Matrix4 cameraRotation = Matrix4(camera->GetViewVector(), camera->GetRightVector(), camera->GetUpVector());
        return camera->GetPosition() + cameraRotation * GetPosition();
    }


    bool Wand::IsTracked()
    {
        return _tracked;
    }


    int Wand::GetNumButtons(void)
    {
        return _numButtons;
    }


    bool Wand::IsButtonPressed(int i)
    {
        int size = sizeof(_buttons) / sizeof(int);
        if (i >= size)
            return false;
        return (_buttons[i] == 1);
    }


    double Wand::GetJoystickHorizontal()
    {
        return _joystickHorizontal;
    }


    double Wand::GetJoystickVertical()
    {
        return _joystickVertical;
    }


    bool Wand::IsZButtonPressed()
    {
        return _buttons[0];
    }


    bool Wand::IsRightButtonPressed()
    {
        return _buttons[1];
    }


    bool Wand::isMiddleButtonPressed()
    {
        return _buttons[2];
    }


    bool Wand::isLeftButtonPressed()
    {
        return _buttons[3];
    }


    Wand Wand::GetCopy(void)
    {
        Wand w;
        w._tracked = _tracked;
        w._position = Vector3(_position.GetX(), _position.GetY(), _position.GetZ());
        w._view = Vector3(_view.GetX(), _view.GetY(), _view.GetZ());
        w._up = Vector3(_up.GetX(), _up.GetY(), _up.GetZ());
        w._right = Vector3(_right.GetX(), _right.GetY(), _right.GetZ());

        w._numButtons = _numButtons;
    
        for (int i = 0; i < _numButtons; i++)
            w._buttons[i] = _buttons[i];

        w._joystickHorizontal = _joystickHorizontal;
        w._joystickVertical = _joystickVertical;
    
        w._previousPositions = _previousPositions;
        w._previousViews = _previousViews;

        w._rollingAverage = _rollingAverage;

        return w;
    }

}