#include "TrackerUpdate.h"

namespace MTF
{

    TrackerUpdate::TrackerUpdate(int port)
    {
        _port = port;
        _stopRequested = false;

        _head = new Head();
        _wand = new Wand();
    }


    TrackerUpdate::TrackerUpdate(int port, int smoothing)
    {
        _port = port;
        _stopRequested = false;

        _head = new Head();
        _wand = new Wand(smoothing);
    }


    TrackerUpdate::~TrackerUpdate(void)
    {
        delete _head;
        delete _wand;
    }


    void TrackerUpdate::Run() 
    {
        assert(!_thread);
        _thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&TrackerUpdate::Update, this)));
    }


    void TrackerUpdate::Stop()
    {
        assert(_thread);
        _stopRequested = true;
        _thread->join();
    }


    void TrackerUpdate::Update()
    {
        DTrackSDK dt("", 0, _port, DTrackSDK::SYS_DTRACK_UNKNOWN);
        if (!dt.isUDPValid())
        {
            perror("\nUnable to recieve data from the ART Tracker.  You may want to check if:\n\tART Tracker is turned on and is tracking (2 red LEDs per camera)\n\tThe correct port number has been specified\n\tWindows Firewall is not blocking the traffic\n\tART Tracker is configured to send tracking updates to your IP address\n\tYou do not currently have another application running on this computer using the tracker\n");
            exit(-1);
        }

        while (!_stopRequested)
        {
            bool ok = dt.receive();

            if (ok) 
            {
                boost::mutex::scoped_lock l(_mutex);
                _head->Update(*dt.getBody(0));
                _wand->Update(*dt.getFlyStick(0));
            }
            else
            {
            }
        }
    }


    Head TrackerUpdate::GetHead()
    {
        boost::mutex::scoped_lock l(_mutex);
        return _head->GetCopy();
    }


    Wand TrackerUpdate::GetWand()
    {
        boost::mutex::scoped_lock l(_mutex);
        return _wand->GetCopy();
    }


    bool TrackerUpdate::IsRunning()
    {
        return !_stopRequested;
    }
}