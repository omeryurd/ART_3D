#ifndef _TRACKINGBODY_H
#define _TRACKINGBODY_H
///
///  \file TrackingBody.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::TrackingBody TrackingBody.h "TrackingBody.h"
///  \brief This class contains a pure virtual interface for tracking bodies.
///
///  Any tracking devices used in the framework must implement the TrackingBody
///  interface.  This interface defines a few key methods that all tracking
///  devices share in common.  
///

#include "Vector3.h"
#include "Matrix4.h"

namespace MTF
{

    class TrackingBody
    {

    public:
        ///
        ///  \brief Pure virtual method for retrieving the position of a tracking device
        ///
        ///  \return                        Vector3 containing the position of the tracking device
        ///
        virtual Vector3 GetPosition()=0;

        ///
        ///  \brief Pure virtual method for retrieving the view direction of a tracking device
        ///
        ///  \return                        Vector3 containing the view vector of the tracking device
        ///
        virtual Vector3 GetViewVector()=0;

        ///
        ///  \brief Pure virtual method for retrieving the up direction of a tracking device
        ///
        ///  \return                        Vector3 containing the up direction of the tracking device
        ///
        virtual Vector3 GetUpVector()=0;

        ///
        ///  \brief Pure virtual method for retrieving the right direction of a tracking device
        ///
        ///  \return                        Vector3 containing the right direction of the tracking device
        ///
        virtual Vector3 GetRightVector()=0;

        ///
        ///  \brief Pure virtual method for retrieving a transform matrix for a tracking device's position and orientation
        ///
        ///  \return                        Matrix4 containing the transform matrix of the tracking device
        ///
        virtual Matrix4 GetTransformMatrix()=0;

        ///
        ///  \brief Pure virtual method for checking if a tracking device is currently tracked
        ///
        ///  \return                        A bool that indicates if the tracking device is being tracked
        ///
        virtual bool IsTracked()=0;
    };

}

#endif