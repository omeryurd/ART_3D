#ifndef _TRACKERUPDATE_H
#define _TRACKERUPDATE_H
///
///  \file TrackerUpdate.h
///  \author  Ben Aeschliman <aescbd01@ipfw.edu>
///  \version 1.0
///
///  \class MTF::TrackerUpdate TrackerUpdate.h "TrackerUpdate.h"
///  \brief This class handles communication with the ART Tracker and updates the Head and Wand objects.
///
///  This class is designed to only be used from the Monolith class.  So, no methods
///  are accessible here by using the framework.  There is no need to try to call any
///  methods here, since all the functionality is available though Monolith.
///

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "DTrackSDK.hpp"

#include "Head.h"
#include "Wand.h"

namespace MTF
{

class TrackerUpdate
{
    // The code under this private block can be public, but we don't want users
    // who implement the framework to be able to call these functions, so we have
    // to use the friend class approach
    private:
        friend class Monolith;

        TrackerUpdate(int port);
        TrackerUpdate(int port, int smoothing);
        ~TrackerUpdate();
 
        void Run();
        void Stop();

        Head GetHead();
        Wand GetWand();

        bool IsRunning();

    private:

        void Update();

        int _port;

        volatile bool _stopRequested;
        boost::shared_ptr<boost::thread> _thread;
        boost::mutex _mutex;

        Head *_head;
        Wand *_wand;
    };

}

#endif