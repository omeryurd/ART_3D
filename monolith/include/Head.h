#ifndef _Head_H
#define _Head_H
///
///  \file Head.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Head Head.h "Head.h"
///  \brief This class handles head tracking for the framework
///
///  This class derives from TrackingBody and implements the methods
///  needed to perform head tracking operations.
///

#include "Display.h"
#include "DTrackSDK.hpp"

namespace MTF
{

    class Head : TrackingBody
    {

    public:
        ///
        ///  \brief Head Constructor
        ///
        ///  Creates a Head object defaulting to not tracked, a position five feet
        ///  out and up from the screen's origin, looking directly at the screen.
        ///
        Head();

        ///
        ///  \brief Head Deconstructor
        ///
        ~Head();

        ///
        ///  \brief Returns the current position of the head
        ///
        ///  \return                        Vector3 containing the position of the head
        ///
        Vector3 GetPosition();

        ///
        ///  \brief Returns the current view direction of the head
        ///
        ///  \return                        Vector3 containing the view direction of the head
        ///
        Vector3 GetViewVector();

        ///
        ///  \brief Returns the current up direction of the head
        ///
        ///  \return                        Vector3 containing the up direction of the head
        ///
        Vector3 GetUpVector();

        ///
        ///  \brief Returns the current right direction of the head
        ///
        ///  \return                        Vector3 containing the right direction of the head
        ///
        Vector3 GetRightVector();

        ///
        ///  \brief Returns the current transform matrix for the head
        ///
        ///  \return                        Matrix4 containing the transform matrix for the head
        ///
        Matrix4 GetTransformMatrix();

        ///
        ///  \brief Returns whether the Head is being tracked or not
        ///
        ///  \return                        A bool indicating if the Head is tracked or not
        ///
        bool IsTracked();

        ///
        ///  \brief Updates the values of the Head's position and orientation
        ///
        void Update(DTrack_Body_Type_d data);

        ///
        ///  \brief Returns a copy of this Head
        ///
        ///  \return                        A copy of the current Head
        ///
        Head GetCopy();

    private:
        bool _tracked;

        Vector3 _position;
        Vector3 _view;
        Vector3 _up;
        Vector3 _right;
    };

}

#endif