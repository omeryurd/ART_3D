#ifndef _MONOLITH_H
#define _MONOLITH_H
///
///  \file Monolith.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Monolith Monolith.h "Monolith.h"
///  \brief This class is the primary interface with the Monolith Tracking Framework
///
///  This class provides methods to get the current Camera, Head, and Wand objects.
///  It is also responsible for starting the tracker updates and turning them off.
///
///
///  \mainpage Monolith Overview
///
///  \section intro_sec Introduction
///  The Monolith Framework can be used to create a stereoscopic environment with input
///  from 6DOF devices such as a head or wand tracking device.
///
///  \section content_sec Contents
///  This package consists of one main class: Monolith.  For new applications, use this class.
///  Also provided are secondary classes to perform common functions.  These include Camera and display
///  for setting up the environment, Matrix4 and Vector3 for math, and Head and Wand for handling
///  tracking information.  The current Camera, Display, Head, and Wand in use can be retrived using methods
///  in the Monolith class.  All classes of the Monolith framework use the MTF namespace.
///

#include "TrackerUpdate.h"
#include "Camera.h"
#include "Head.h"
#include "Wand.h"

namespace MTF
{

    class Monolith
    {

    public:
        ///
        ///  \brief Monolith Constructor
        ///
        ///  This will create a Monolith object with the given camera object
        ///  and port number.  Smoothing for the wand will be disabled.
        ///
        ///  \param camera                  Camera object that contains the inital camera to be used in the framework
        ///  \param port                    The port number the ART Tracker is listening on
        ///
	    Monolith(Camera *camera, int port);

        ///
        ///  \brief Monolith Constructor
        ///
        ///  This will create a Monolith object with the given camera object, port number,
        ///  and amount of smoothing.
        ///
        ///  \param camera                  Camera object that contains the inital camera to be used in the framework
        ///  \param port                    The port number the ART Tracker is listening on
        ///  \param smoothing               Number of previous updates to include when average the wand's position and view vectors
        ///
        Monolith(Camera *camera, int port, int smoothing);

        ///
        ///  \brief Monolith Deconstructor
        ///
	    ~Monolith();

        ///
        ///  \brief Retrieve a copy of the current Head object
        ///
        ///  \return                        A copy of the current Head object
        ///
        Head GetHead();

        ///
        ///  \brief Retrieve a copy of the current Wand object
        ///
        ///  \return                        A copy of the current Wand object
        ///
        Wand GetWand();

        ///
        ///  \brief Retrieve the Camera object
        ///
        ///  This Camera will store the position and orientation that is used
        ///  for rendering.  This camera should be modified if you wish to make
        ///  changes in the camera.  This camera should also be used to retrieve
        ///  valid view and projection matrices.
        ///
        ///  \return                        The Camera object used by the framework
        ///
        Camera* GetCamera();

        ///
        ///  \brief Retrieve the Display object
        ///
        ///  \return                        The Display object used by the framework
        ///
        Display* GetDisplay();

        ///
        ///  \brief Stop processing tracker updates and threads associated with tracking
        ///
        void ShutdownTracking();

        ///
        ///  \brief Check if TrackerUpdate is running
        ///
        ///  \return                        Returns whether the tracker updates will be processed or not
        ///
        bool IsRunning();

    private:
        TrackerUpdate *_tracker;
        Camera *_camera;

        bool _running;
    };

}

#endif