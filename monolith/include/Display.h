#ifndef _DISPLAY_H
#define _DISPLAY_H
///
///  \file Display.cpp
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Display Display.h "Display.h"
///  \brief This class contains physical information about the display device.
///
///  This class stores the corner locations of a display device.  These locations
///  give us the width and height of the screen, as well as the up, right, and out
///  vectors of the screen.  These locations and vectors are used by the Camera class
///  to create the view and projection matrices for tracking and stereo rendering.
///

#include "TrackingBody.h"

namespace MTF
{

    class Display
    {

    public:
        ///
        ///  \brief Display Constructor
        ///
        ///  Creates a display object from the given parameters.  
        ///  The display contains the physical parameters of the screen size and location.
        ///
        ///  @param lowerLeftCorner         Vector3 location of the lower left corner of the screen in feet
        ///  @param lowerRightCorner        Vector3 location of the lower right corner of the screen in feet
        ///  @param upperLeftCorner         Vector3 location of the upper left corner of the screen in feet
        ///
        Display(Vector3 lowerLeftCorner, Vector3 lowerRightCorner, Vector3 upperLeftCorner);

        ///
        ///  \brief Display Destructor
        ///
        ~Display();
    
        ///
        ///  \brief The up vector for the screen
        ///
        ///  @return                        Vector3 containing the normalized up vector for the screen
        ///
        Vector3 GetScreenUpVector();

        ///
        ///  \brief The right vector for the screen
        ///
        ///  @return                        Vector3 containing the normalized right vector for the screen
        ///
        Vector3 GetScreenRightVector();

        ///
        ///  \brief The out vector for the screen
        ///
        ///  @return                        Vector3 containing the normalized out (toward viewer) vector for the screen
        ///
        Vector3 GetScreenOutVector();

        ///
        ///  \brief The lower left corner of the screen
        ///
        ///  @return                        Vector3 containing the position of the lower left corner of the screen in feet
        ///
        Vector3 GetLowerLeftCorner();

        ///
        ///  \brief Returns the lower right corner of the screen
        ///
        ///  @return                        Vector3 containing the position of the lower right corner of the screen in feet
        ///
        Vector3 GetLowerRightCorner();

        ///
        ///  \brief Returns the upper left corner of the screen
        ///
        ///  @return                        Vector3 containing the position of the upper left corner of the screen in feet
        ///
        Vector3 GetUpperLeftCorner();

        ///
        ///  \brief Returns the upper right corner of the screen
        ///
        ///  @return                        Vector3 containing the position of the upper right corner of the screen in feet
        ///
        Vector3 GetUpperRightCorner();

        ///
        ///  \brief Returns the width of the screen
        ///
        ///  @return                        double value for the width of the screen in feet
        ///
        double GetScreenWidth();

        ///
        ///  \brief Returns the height of the screen
        ///
        ///  @return                        double value for the height of the screen in feet
        ///
        double GetScreenHeight();

        ///
        ///  \brief Returns a matrix for the orientation of the screen
        ///
        ///  @return                        A Matrix4 which contains the rotation information for the screen
        ///
        Matrix4 GetMwMatrix();

    private:
        ///  Matrix for the screen orientation
        Matrix4 _MwMatrix;

        ///  Real world position of the screen's lower left corner
        Vector3 _LL;
        ///  Real world position of the screen's lower right corner
        Vector3 _LR;
        ///  Real world position of the screen's upper left corner
        Vector3 _UL;

        /// Real world display screen width
        double _width;
        /// Real world display screen height
        double _height;
    };
}

#endif
