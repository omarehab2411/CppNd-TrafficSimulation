#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lck(_mtx);
    
    _condition.wait(lck,[this] { return !_queue.empty(); });
    
    T rxMsg = std::move(_queue.front());
    
    _queue.pop_front();
    
    
    return rxMsg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lck(_mtx);
    _queue.push_back(std::move(msg));
    _condition.notify_one();
    
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

TrafficLight::~TrafficLight()
{    
}

void TrafficLight::WaitForGreen()
{
    TrafficLightPhase currentPhase;
    while(1)
    {
     currentPhase = _msgQueue.receive();
     if(currentPhase == green)
     {
         break;
     }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

double TrafficLight::getCycleDuration (void)
{
        //rand()%(max-min + 1) + min
        return (rand()%(6000-4000 + 1) + 4000);
};

void TrafficLight::Simulate()
{
     threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    double cycleDuration = getCycleDuration();
    
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    lastTime = std::chrono::system_clock::now();
    
    while(1)
    {
     std::this_thread::sleep_for(std::chrono::milliseconds(1));
     long cycleDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastTime).count();
      if(cycleDiff >= cycleDuration)
      {   
          if(getCurrentPhase() == red)
          {  
              _currentPhase = green ;
              
          }
          else if(getCurrentPhase() == green)
          {     
               _currentPhase = red ;
                 
          }
          TrafficLightPhase phase = _currentPhase;
          _msgQueue.send( std::move(phase) );
           

          lastTime = std::chrono::system_clock::now();
          cycleDuration = getCycleDuration();
          
      }
    }
}

