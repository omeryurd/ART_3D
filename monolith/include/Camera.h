#ifndef _CAMERA_H
#define _CAMERA_H
///
///  \file Camera.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Camera Camera.h "Camera.h"
///  \brief This class contains the camera that should be used for navigating a scene.
///
///  This class provides a way to specify the position and orientation of a camera.  Based
///  on this information, one can acquire the view and projection matrices for a left, right, or
///  monoscopic eye.  These Matrix4 matrices should be used when rendering to produce the proper
///  stereoscopic and/or head tracking experience.
///
///  If you are using another library to handle the camera movements, then you must do one of the
///  following:  Copy the other library's camera parameters (position, view, up) to the Monolith camera
///  or copy the monolith camera parameters to the library's camera.  Failure to do this will either
///  mean head/wand tracking will not work properly, or the rendering on the screen won't match the
///  head/wand position or orientation.
///

#include "Display.h"
#include "Eye.h"

#include <iostream>

namespace MTF
{

    class Camera
    {

    public:
        ///
        ///  \brief Camera Constructor
        ///
        ///  Creates a camera object from the given parameters.  
        ///  The camera's position defaults to the origin and the yaw and pitch default to zero.
        ///
        ///  @param display                Pointer to Display object containing screen information
        ///  @param nearPlane              Double value for the near plane distance.  Must be a positive value.
        ///  @param farPlane               Double value for the far plane distance.  Must be greater than the near plane
        ///
        Camera(Display *display, double nearPlane, double farPlane);

        ///
        ///  \brief Camera Constructor
        ///
        ///  Creates a camera object from the given parameters.
        ///  This constructor allows the setting of all modifiable parameters that influence the camera.
        ///
        ///  @param display                 Pointer to Display object containing screen information
        ///  @param nearPlane               Double value for the near plane distance.  Must be a positive value.
        ///  @param farPlane                Double value for the far plane distance.  Must be greater than the near plane
        ///  @param cameraPosition          Vector3 that contains the x, y, and z values for the camera's initial position
        ///  @param yaw                     Double value that contains the amount of rotation around the Y-axis.  Value is expected in degrees.
        ///  @param pitch                   Double value that contains the amount of rotation around the X-axis.  Value is expected in degrees.
        ///
        Camera(Display *display, double nearPlane, double farPlane, Vector3 cameraPosition, double yaw, double pitch);

        ///
        ///  \brief Camera Destructor
        ///
        ~Camera();

        ///
        ///  \brief Returns a view matrix for the given parameters.
        ///
        ///  This method should be used when you are performing a stereoscopic rendering and wish to render
        ///  the correct perspective and eye seperation based on a tracking device.
        ///
        ///  @param eye                     An enum from the Eye class which indicates which eye we are wanting to create a view matrix for.
        ///  @param trackingBody            A pointer to a TrackingBody object.  You can pass in either the head or wand, once casted properly.
        ///
        ///  @return                        A Matrix4 object which contains a valid view matrix for the given parameters
        ///
        Matrix4 GetViewMatrix(Eye::EYETYPE eye, TrackingBody *trackingBody);

        ///
        ///  \brief Returns a view matrix for the given parameters.
        ///
        ///  This method should be used when you are performing a stereoscopic rendering or monoscopic rendering
        ///  and head tracking is not needed
        ///
        ///  @param eye                     An enum from the Eye class which indicates which eye we are wanting to create a view matrix for.
        ///
        ///  @return                        A Matrix4 object which contains a valid view matrix for the given parameters
        ///
        Matrix4 GetViewMatrix(Eye::EYETYPE eye);

        ///
        ///  \brief Returns a view matrix for monoscopic rendering.
        ///
        ///  This method is only useful for monoscopic rendering without head tracking
        ///
        ///  @return                        A Matrix4 object which contains a valid view matrix for the given parameters
        ///
        Matrix4 GetViewMatrix();

        ///
        ///  \brief Returns a projection matrix for the given parameters.
        ///
        ///  This method should be used when you are performing a stereoscopic rendering and wish to render
        ///  the correct perspective and eye seperation based on a tracking device.
        ///
        ///  @param eye                     An enum from the Eye class which indicates which eye we are wanting to create a projection matrix for.
        ///  @param trackingBody            A pointer to a TrackingBody object.  You can pass in either the head or wand, once casted properly.
        ///
        ///  @return                        A Matrix4 object which contains a valid projection matrix for the given parameters
        ///
        Matrix4 GetProjectionMatrix(Eye::EYETYPE eye, TrackingBody *trackingBody);

        ///
        ///  \brief Returns a projection matrix for the given parameters.
        ///
        ///  This method should be used when you are performing a stereoscopic rendering or monoscopic rendering
        ///  and head tracking is not needed
        ///
        ///  @param eye                     An enum from the Eye class which indicates which eye we are wanting to create a projection matrix for.
        ///
        ///  @return                        A Matrix4 object which contains a valid projection matrix for the given parameters
        ///
        Matrix4 GetProjectionMatrix(Eye::EYETYPE eye);

        ///
        ///  \brief Returns a projection matrix for the given parameters.
        ///
        ///  This method is only useful for monoscopic rendering without head tracking
        ///
        ///  @return                        A Matrix4 object which contains a valid projection matrix for the given parameters
        ///
        Matrix4 GetProjectionMatrix();

        ///
        ///  \brief Sets the pitch for the camera.
        ///
        ///  @param pitch                   Expected value is the degrees of rotation (clockwise) around the +x axis
        ///
        void SetPitch(double pitch);

        ///
        ///  \brief Sets the yaw for the camera.
        ///
        ///  @param yaw                     Expected value is the degrees of rotation (counter-clockwise) around the +y axis
        ///
        void SetYaw(double yaw);

        ///
        ///  \brief Sets the position of the camera
        ///
        ///  @param position                Position of the camera in object space
        ///
        void SetPosition(Vector3 position);

        ///
        ///  \brief Adjusts the pitch of the camera
        ///
        ///  The value passed in for pitch is added to the camera's current pitch.
        ///
        ///  @param pitch                   Expected value is the degrees of rotation (clockwise) around the +x axis
        ///
        void AdjustPitch(double pitch);

        ///
        ///  \brief Sets the yaw for the camera.
        ///
        ///  The value passed in for yaw is added to the camera's current yaw.
        ///
        ///  @param yaw                     Expected value is the degrees of rotation (counter-clockwise) around the +y axis
        ///
        void AdjustYaw(double yaw);

        /// 
        ///  \brief Moves the position of the camera forward.
        ///
        ///  The position of the camera is moved forward by the amount specified in the direction of the current view vector.
        ///
        ///  @param amount                  Number of units to move the camera forward.
        ///
        void MoveForward(double amount);

        /// 
        ///  \brief Moves the position of the camera forward.
        ///
        ///  The position of the camera is moved forward by the amount specified in the direction of the current view vector.
        ///
        ///  @param amount                  Number of units to move the camera forward.
        ///
        void MoveBackward(double amount);

        /// 
        ///  \brief Moves the position of the camera to the right.
        ///
        ///  The position of the camera is moved right by the amount specified in the direction of the current right vector.
        ///
        ///  @param amount                  Number of units to move the camera to the right.
        ///
        void MoveRight(double amount);

        /// 
        ///  \brief Moves the position of the camera to the left.
        ///
        ///  The position of the camera is moved left by the amount specified in the direction opposite of the current right vector.
        ///
        ///  @param amount                  Number of units to move the camera to the left.
        ///
        void MoveLeft(double amount);

        /// 
        ///  \brief Moves the position of the camera up.
        ///
        ///  The position of the camera is moved up by the amount specified in the direction of the current up vector.
        ///
        ///  @param amount                  Number of units to move the camera up.
        ///
        void MoveUp(double amount);

        /// 
        ///  \brief Moves the position of the camera down.
        ///
        ///  The position of the camera is moved down by the amount specified in the opposite direction of the current up vector.
        ///
        ///  @param amount                  Number of units to move the camera down.
        ///
        void MoveDown(double amount);

        /// 
        ///  \brief Returns the camera's current pitch.
        ///
        ///  @return                        The current pitch for the camera in degrees
        ///
        double GetPitch();

        /// 
        ///  \brief Returns the camera's current yaw.
        ///
        ///  @return                        The current yaw for the camera in degrees
        ///
        double GetYaw();

        /// 
        ///  \brief Returns the camera's current position.
        ///
        ///  @return                        The camera's current position in object space
        ///
        Vector3 GetPosition();

        /// 
        ///  \brief Returns the camera's current view vector.
        ///
        ///  @return                        The camera's current view vector (direction camera is facing) in object space
        ///
        Vector3 GetViewVector();

        /// 
        ///  \brief Returns the camera's current up vector.
        ///
        ///  @return                        The camera's current up vector in object space
        ///
        Vector3 GetUpVector();

        /// 
        ///  \brief Returns the camera's current right vector.
        ///
        ///  @return                        The camera's current right vector in object space
        ///
        Vector3 GetRightVector();

        /// 
        ///  \brief Returns the Display that has been assigned to the camera.
        ///
        ///  @return                        A pointer to the Display assigned to this camera
        ///
        Display* GetDisplay();

        /// 
        ///  \brief Sets this camera to the passed in parameters.
        ///
        ///  This method can be used when synchronizing this camera with a camera
        ///  not part of the Monolith framework.
        ///
        ///  @param position                The position the camera should be located
        ///  @param view                    The direction the camera is to face
        ///  @param right                   The direction to the right of the camera (should be orthogonal to the view)
        ///  @param up                      The direction up from the camera (should be orthogonal to the view and right vectors)
        ///
        void SetCamera(Vector3 position, Vector3 view, Vector3 right, Vector3 up);

    private:
        Vector3 _cameraPosition;
        Vector3 _cameraViewVector;
        Vector3 _cameraUpVector;
        Vector3 _cameraRightVector;

        double _yaw;
        double _pitch;

        double _nearPlane;
        double _farPlane;

        double _left;
        double _right;
        double _top;
        double _bottom;

        Display *_display;

        void RecalculateCameraVectors();

        Matrix4 GetViewMatrix(Eye::EYETYPE eye, Vector3 eyePos);
        Matrix4 GetProjectionMatrix(Eye::EYETYPE eye, Vector3 eyePos);
    };
}

#endif