#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

enum TrafficLightPhase 
{
    red = 0,
    green
};

// forward declarations to avoid include cycle
class Vehicle;


template <class T>
class MessageQueue
{
public:
void send (T &&message);
T receive ();
private:
    std::deque<T>  _queue; 
    std::condition_variable _condition;
    std::mutex _mtx;
    
};

 

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
      TrafficLight();
      ~TrafficLight();
       

    // getters / setters
    TrafficLightPhase getCurrentPhase (void);
    double getCycleDuration (void);
    // typical behaviour methods
    void WaitForGreen();
    void Simulate ();

private:
    // typical behaviour methods
    void cycleThroughPhases (void);
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _msgQueue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif