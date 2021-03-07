#include "Camera.h"

namespace MTF
{
    /// Interocular distance (approximately 2.5 inches) expressed in feet
    const double IOD = 0.21;  

    /// Approximate value for PI
    const double PI  = 3.14159265;


    Camera::Camera(Display *display, double nearPlane, double farPlane)
    {
        _display = display;

        _nearPlane = nearPlane;
        _farPlane  = farPlane;

        _cameraPosition    = Vector3::ZERO;
        _cameraViewVector  = Vector3::NEGATIVE_UNIT_Z;
        _cameraUpVector    = Vector3::UNIT_Y;
        _cameraRightVector = Vector3::UNIT_X;

        _top = _bottom = _left = _right = 0;
        _yaw = _pitch = 0;
    }


    Camera::Camera(Display *display, double nearPlane, double farPlane, Vector3 cameraPosition, double yaw, double pitch)
    {
        _display = display;

        _nearPlane = nearPlane;
        _farPlane  = farPlane;

        _cameraPosition    = Vector3::ZERO;
        _cameraViewVector  = Vector3::NEGATIVE_UNIT_Z;
        _cameraUpVector    = Vector3::UNIT_Y;
        _cameraRightVector = Vector3::UNIT_X;

        _top = _bottom = _left = _right = 0;
        _yaw = yaw * PI / 180;
        _pitch = pitch * PI / 180;

        RecalculateCameraVectors();
    }


    Camera::~Camera()
    {
    }


    // Returns a view matrix for the given display, eye, and tracking body
    Matrix4 Camera::GetViewMatrix(Eye::EYETYPE eye, TrackingBody *trackingBody)
    {
        Vector3 eyeVector = trackingBody->GetPosition();
        eyeVector *= Vector3(1.0, 1.0, -1.0);  // Flip the Z axis

        Matrix4 rotation;
        rotation.MakeRotationMatrix(_cameraViewVector, _cameraRightVector, _cameraUpVector);
        eyeVector  = rotation * eyeVector;

        Vector3 eyeOffset = trackingBody->GetRightVector() * IOD / 2;
        eyeOffset = rotation * eyeOffset;

        if (eye == Eye::LEFT)
        {
            eyeVector -= eyeOffset;
        }
        else if (eye == Eye::RIGHT)
        {
            eyeVector += eyeOffset;
        }

        return GetViewMatrix(eye, eyeVector);
    }


    // Returns a view matrix for the given eye
    Matrix4 Camera::GetViewMatrix(Eye::EYETYPE eye)
    {
        Vector3 eyeVector = Vector3::ZERO;

        Vector3 eyeOffset = _cameraRightVector;
        eyeOffset *= IOD / 2;

        if (eye == Eye::LEFT)
        {
            eyeVector -= eyeOffset;
        }
        else if (eye == Eye::RIGHT)
        {
            eyeVector += eyeOffset;
        }

        return GetViewMatrix(eye, eyeVector);
    }


    // Returns the view matrix based off only the camera, for a monoscopic display
    Matrix4 Camera::GetViewMatrix()
    {
        return GetViewMatrix(Eye::MONO);
    }


    // For a given eye type and eye position, return a view matrix
    Matrix4 Camera::GetViewMatrix(Eye::EYETYPE eye, Vector3 eyePos)
    {
        Matrix4 viewMatrix;
        viewMatrix.MakeViewMatrix(_cameraPosition + eyePos, _cameraViewVector, _cameraRightVector, _cameraUpVector);
        return viewMatrix;
    }


    // Returns the projection matrix for an associated display, eye type and tracking body
    Matrix4 Camera::GetProjectionMatrix(Eye::EYETYPE eye, TrackingBody *trackingBody)
    {
        Vector3 eyeVector = Vector3(trackingBody->GetPosition().GetX(),
                                    trackingBody->GetPosition().GetY(),
                                    trackingBody->GetPosition().GetZ());

        Vector3 eyeOffset = trackingBody->GetRightVector() * IOD / 2;

        if (eye == Eye::LEFT)
        {
            eyeVector += eyeOffset;
        }
        else if (eye == Eye::RIGHT)
        {
            eyeVector -= eyeOffset;
        }
    
        Vector3 Es = (eyeVector - _display->GetLowerLeftCorner());

        return GetProjectionMatrix(eye, Es);
    }


    // Returns the projection matrix for an associated display and eye type, no tracking
    Matrix4 Camera::GetProjectionMatrix(Eye::EYETYPE eye)
    {
        // This eye Vector makes the assumption that the origin is centered horizontally
        // at the bottom of the screen (which is what our projection system uses)
        Vector3 eyeVector = (_display->GetUpperLeftCorner() - _display->GetLowerLeftCorner()) * 0.5 + 
                             _display->GetScreenOutVector() * 5;
    
        Vector3 eyeOffset = _display->GetScreenRightVector() * IOD / 2;

        if (eye == Eye::LEFT)
        {
            eyeVector += eyeOffset;
        }
        else if (eye == Eye::RIGHT)
        {
            eyeVector -= eyeOffset;
        }
    
        Vector3 Es = (eyeVector - _display->GetLowerLeftCorner());

        return GetProjectionMatrix(eye, Es);
    }


    // Returns the projection matrix for an associated display, no tracking or stereo
    Matrix4 Camera::GetProjectionMatrix()
    {
        return GetProjectionMatrix(Eye::MONO);
    }


    Matrix4 Camera::GetProjectionMatrix(Eye::EYETYPE eye, Vector3 eyePos)
    {
        double L = eyePos.DotProduct(_display->GetScreenRightVector());
        double R = _display->GetScreenWidth() - L;
        double B = eyePos.DotProduct(_display->GetScreenUpVector());
	    double T = _display->GetScreenHeight() - B;

        double distance = eyePos.AbsDotProduct(_display->GetScreenOutVector()); // This is also the focal length

        double left   = -L * _nearPlane / distance;
        double right  =  R * _nearPlane / distance;
        double bottom = -B * _nearPlane / distance;
        double top    =  T * _nearPlane / distance;

        double a = (2.0f * _nearPlane) / (right - left);
	    double b = (right + left) / (right - left);
	    double c = (2.0f * _nearPlane) / (top - bottom);
	    double d = (top + bottom) / (top - bottom);
	    double e = -1.0f * (_farPlane + _nearPlane) / (_farPlane - _nearPlane);
	    double f = (-2.0f * _farPlane * _nearPlane) / (_farPlane - _nearPlane);
	    double g = -1.0f;
	
	    Matrix4 proj = Matrix4(a, 0, b, 0,
		                       0, c, d, 0,
                               0, 0, e, f,
                               0, 0, g, 0);

        return proj;
    }


    void Camera::SetPitch(double pitch)
    {
        _pitch = pitch * PI / 180;
        RecalculateCameraVectors();
    }


    void Camera::SetYaw(double yaw)
    {
        _yaw = yaw * PI / 180;
        RecalculateCameraVectors();
    }


    void Camera::AdjustPitch(double pitch)
    {
        _pitch += pitch * PI / 180;
        RecalculateCameraVectors();
    }
    

    void Camera::AdjustYaw(double yaw)
    {
        _yaw += yaw * PI / 180;
        RecalculateCameraVectors();
    }

 
    double Camera::GetPitch()
    {
        return _pitch * 180 / PI;
    }


    double Camera::GetYaw()
    {
        return _yaw * 180 / PI;
    }


    void Camera::MoveForward(double amount)
    {
        _cameraPosition += _cameraViewVector * amount;
    }


    void Camera::MoveBackward(double amount)
    {
        _cameraPosition -= _cameraViewVector * amount;
    }


    void Camera::MoveRight(double amount)
    {
        _cameraPosition += _cameraRightVector * amount;
    }


    void Camera::MoveLeft(double amount)
    {
        _cameraPosition -= _cameraRightVector * amount;
    }


    void Camera::MoveUp(double amount)
    {
        _cameraPosition += _cameraUpVector * amount;
    }


    void Camera::MoveDown(double amount)
    {
        _cameraPosition -= _cameraUpVector * amount;
    }


    void Camera::SetPosition(Vector3 position)
    {
        _cameraPosition = position;
    }


    Vector3 Camera::GetPosition()
    {
        return _cameraPosition;
    }


    Vector3 Camera::GetViewVector()
    {
        return _cameraViewVector;
    }


    Vector3 Camera::GetUpVector()
    {
        return _cameraUpVector;
    }


    Vector3 Camera::GetRightVector()
    {
        return _cameraRightVector;
    }


    Display* Camera::GetDisplay()
    {
        return _display;
    }


    void Camera::SetCamera(Vector3 position, Vector3 view, Vector3 right, Vector3 up)
    {
        _cameraPosition = position.GetNormalized();
        _cameraViewVector = view.GetNormalized();
        _cameraRightVector = right.GetNormalized();
        _cameraUpVector = up.GetNormalized();

        // Recalculate pitch and yaw
        _yaw = atan2(view.GetZ(), view.GetX());
        _pitch = asin(view.GetY());
    }


    void Camera::RecalculateCameraVectors()
    {
        // Make pitch and yaw fall between 0 and 2*PI
        while (_pitch < 0)
            _pitch += 2*PI;
        while (_pitch >= 2*PI)
            _pitch -= 2*PI;

        while (_yaw < 0)
            _yaw += 2*PI;
        while (_yaw >= 2*PI)
            _yaw -= 2*PI;

        // Create a rotation matrix from the pitch and yaw
        Matrix4 pitchMatrix;
        Matrix4 yawMatrix;

        pitchMatrix.MakePitchRotationMatrix(_pitch);
        yawMatrix.MakeYawRotationMatrix(_yaw);

        Matrix4 rotation = yawMatrix * pitchMatrix;

        // Rotate the orignal view/right/up vectors to be correct for the given yaw and pitch
        _cameraViewVector = (rotation * Vector3::NEGATIVE_UNIT_Z).GetNormalized();
        _cameraUpVector = (rotation * Vector3::UNIT_Y).GetNormalized();
        _cameraRightVector = (rotation * Vector3::UNIT_X).GetNormalized();
    }
}
