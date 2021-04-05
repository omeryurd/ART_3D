#ifndef _WAND_H
#define _WAND_H
///
///  \file Wand.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Wand Wand.h "Wand.h"
///  \brief This class handles wand tracking for the framework
///
///  This class derives from TrackingBody and implements the methods
///  needed to perform wand tracking operations.  While similar to
///  the Head class, the Wand class must also handles joystick
///  and button presses.
///

#include <stdio.h>
#include <iostream>

#include "Display.h"
#include "DTrackSDK.hpp"

#include "Camera.h"

#include "Head.h"

#include <list>

namespace MTF
{

    class Wand : TrackingBody
    {

    public:
        ///
        ///  \brief Wand Constructor
        ///
        ///  Creates a Wand object that defaults to not tracking, a position 
        ///  five feet out and up from the screen's origin, facing the screen,
        ///  with no joystick input or buttons pressed.  Smoothing is disabled
        ///  for the default.
        ///
        Wand();

        ///
        ///  \brief Wand Constructor
        ///
        ///  Creates a Wand object that defaults to not tracking, a position 
        ///  five feet out and up from the screen's origin, facing the screen,
        ///  with no joystick input or buttons pressed.  Smoothing is assigned 
        ///  from the passed in value.
        ///
        ///  \param rollingAverage          Number of previous wand updates to average when calculation the position or direction vectors
        ///
        Wand(int rollingAverage);

        ///
        ///  \brief Wand Deconstructor
        ///
        ~Wand();

        ///
        ///  \brief Returns the current position of the wand
        ///
        ///  \return                        Vector3 containing the position of the wand
        ///
        Vector3 GetPosition();

        ///
        ///  \brief Returns the current view direction of the wand
        ///
        ///  \return                        Vector3 containing the view direction of the wand
        ///
        Vector3 GetViewVector();

        ///
        ///  \brief Returns the current up direction of the wand
        ///
        ///  \return                        Vector3 containing the up direction of the wand
        ///
        Vector3 GetUpVector();

        ///
        ///  \brief Returns the current right direction of the wand
        ///
        ///  \return                        Vector3 containing the right direction of the wand
        ///
        Vector3 GetRightVector();

        ///
        ///  \brief Returns the current transform matrix for the wand
        ///
        ///  \return                        Matrix4 containing the transform matrix for the wand
        ///
        Matrix4 GetTransformMatrix();

        ///
        ///  \brief Returns whether the Wand is being tracked or not
        ///
        ///  \return                        A bool indicating if the Wand is tracked or not
        ///
        bool IsTracked();

        ///
        ///  \brief Updates the values of the Wand's position and orientation
        ///
        void Update(DTrack_FlyStick_Type_d data);

        ///
        ///  \brief Returns the current view direction of the wand in object space
        ///
        ///  \return                        Vector3 containing the view direction of the wand in object space
        ///
        Vector3 GetViewVector(Camera *camera);

        ///
        ///  \brief Returns the current right direction of the wand in object space
        ///
        ///  \return                        Vector3 containing the right direction of the wand in object space
        ///
        Vector3 GetRightVector(Camera *camera);

        ///
        ///  \brief Returns the current position of the wand in object space
        ///
        ///  \return                        Vector3 containing the position of the wand in object space
        ///
        Vector3 GetPosition(Camera *camera);

        ///
        ///  \brief Returns the number of buttons that are being used
        ///
        ///  \return                        An int containing the number of buttons used by this wand
        ///
        int GetNumButtons();

        ///
        ///  \brief Returns if a specific button is pressed
        ///
        ///  \param i                       The index of the button to be checked
        ///  \return                        A bool indicating if the button i is pressed.  If i is greater than the number of buttons, false is returned.
        ///
        bool IsButtonPressed(int i);

        ///
        ///  \brief Returns the value of the horizontal joystick
        ///
        ///  The values for the joystick will range from -1.0 to 1.0.
        ///  It is typical to assume that a certain range (such as -.15 to .15) will be
        ///  in the "dead zone" and not considered when using this value.  The value returned
        ///  from here does not account for this, and is left up to the calling method to
        ///  handle any processing done to the joystick value.
        ///
        ///  \return                        Returns the horizontal component of the joysticks as a double
        ///
        double GetJoystickHorizontal();

        ///
        ///  \brief Returns the value of the vertical joystick
        ///
        ///  The values for the joystick will range from -1.0 to 1.0.
        ///  It is typical to assume that a certain range (such as -.15 to .15) will be
        ///  in the "dead zone" and not considered when using this value.  The value returned
        ///  from here does not account for this, and is left up to the calling method to
        ///  handle any processing done to the joystick value.
        ///
        ///  \return                        Returns the vertical component of the joysticks as a double
        ///
        double GetJoystickVertical();

        ///
        ///  \brief Returns whether the Z button is pressed (Button on back of FlyStick)
        ///
        ///  \return                        A bool indicating the Z button press status
        ///
        bool IsZButtonPressed();

        ///
        ///  \brief Returns whether the right button is pressed
        ///
        ///  \return                        A bool indicating the right button press status
        ///
        bool IsRightButtonPressed();

        ///
        ///  \brief Returns whether the middle button is pressed
        ///
        ///  \return                        A bool indicating the middle button press status
        ///
        bool isMiddleButtonPressed();

        ///
        ///  \brief Returns whether the left button is pressed
        ///
        ///  \return                        A bool indicating the left button press status
        ///
        bool isLeftButtonPressed();

        ///
        ///  \brief Returns a copy of this Wand
        ///
        ///  \return                        A copy of the current Wand
        ///
        Wand GetCopy();
        
    private:
        bool _tracked;

        int _rollingAverage;
        std::list<Vector3> _previousPositions;
        std::list<Vector3> _previousViews;

        Vector3 _position;
        Vector3 _view;
        Vector3 _up;
        Vector3 _right;

        int _numButtons;
        bool _buttons[16];
        double _joystickHorizontal;
        double _joystickVertical;
    };
}

#endif