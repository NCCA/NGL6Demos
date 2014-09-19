#ifndef LEAP_H__
#define LEAP_H__

#include "Leap.h"
#include <ngl/Vec3.h>

class LeapListener : public Leap::Listener
{
  public:
    virtual void onInit(const Leap::Controller&);
    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onExit(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);
    virtual void onDeviceChange(const Leap::Controller&);
    virtual void onServiceConnect(const Leap::Controller&);
    virtual void onServiceDisconnect(const Leap::Controller&);
    std::vector <ngl::Vec3> getFingers() {return m_fingers;}



private :
  std::vector <ngl::Vec3> m_fingers;
};


#endif
