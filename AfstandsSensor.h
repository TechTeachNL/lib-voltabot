#ifndef AfstandsSensor_H
#define AfstandsSensor_H
#include "arduino.h"
#include "VoltaBot.h"
class AfstandsSensor
{
  public:
    AfstandsSensor(int trigPin, int echoPin);
    boolean controleerObstakelOpDrempelwaarde();
    float meetAfstand();
    CheckFunctie voorwerpGedetecteerd();
  private:
    int drempelwaarde;
    int obstakelTeller;
    int trigPin;
    int echoPin;    
};
#endif