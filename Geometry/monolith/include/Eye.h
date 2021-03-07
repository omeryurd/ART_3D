#ifndef _EYE_H
#define _EYE_H
///
///  \file Eye.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::Eye Eye.h "Eye.h"
///  \brief This class contains an enum which is used to specifiy which eye we are focusing on.
///
///  The framework makes use of the eye specified to determine if any offsets should be used
///  when creating the view or projection matricies.  This can also be used in user code to
///  perform different actions based on the current eye that is rendered.
///

namespace MTF
{

    class Eye
    {

    public:
        enum EYETYPE {
            LEFT,                           ///< For calculations that involve the left eye
            RIGHT,                          ///< For calculations that involve the right eye
            MONO,                           ///< For calculations in a monoscopic environment
            NONE                            ///< This can be used to specify a different type of eye for use in your code.  The framework doesn't do any special treatment if this enum is used.
        };

    };

}

#endif
