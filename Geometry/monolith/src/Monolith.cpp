// Using this to check for any memory leaks in a debug build
// You must install Visual Leak Detector if you want to use this
//#include <vld.h>

#include "Monolith.h"

///
///  \brief The MTF namespace houses all of the classes belonging to the Monolith Tracking Framework.
///  The namespace is used since many libraries include classes with the name Camera or Vector3
///  and this allows for an unambiguous way to specify to which is being referred.
///
namespace MTF
{

    Monolith::Monolith(Camera *camera, int port)
    {
        _camera = camera;

        _tracker = new TrackerUpdate(port);
        _tracker->Run();

        _running = true;
    }


    Monolith::Monolith(Camera *camera, int port, int smoothing)
    {
        _camera = camera;

        _tracker = new TrackerUpdate(port, smoothing);
        _tracker->Run();

        _running = true;
    }


    Monolith::~Monolith(void)
    {
        delete _tracker;
    }


    Head Monolith::GetHead()
    {
        return _tracker->GetHead();
    }


    Wand Monolith::GetWand()
    {
        return _tracker->GetWand();
    }


    Camera* Monolith::GetCamera()
    {
        return _camera;
    }


    Display* Monolith::GetDisplay()
    {
        return _camera->GetDisplay();
    }


    void Monolith::ShutdownTracking()
    {
        if (_tracker->IsRunning())
            _tracker->Stop();

        _running = false;
    }


    bool Monolith::IsRunning()
    {
        return _running;
    }
}