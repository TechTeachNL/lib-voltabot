#ifndef VoltaBot_H
#define VoltaBot_H
#include "Arduino.h"
#include <AccelStepper.h>
#include <MultiStepper.h>
#define HALFSTEP 8
#define MAXSENSORCHECKS 10

#define VOORUIT 1
#define ACHTERUIT 2

typedef struct td_SensorActie
{
  void (*actie)();
  boolean (*check)();
} SensorActie;

class VoltaBot
{
  public:
    VoltaBot(int LinksIn1, int LinksIn2, int LinksIn3, int LinksIn4, int RechtsIn1, int RechtsIn2, int RechtsIn3, int RechtsIn4);
    void setupVoltaBot();
    void setStandaardSnelheid(int snelheid);
    void voegSensorCheckToe(int plek, boolean (*check) (), void (*actie)());
    
    void rij();
    void rijVooruit();
    void rijAchteruit();
    void draaiNaarLinks();
    void draaiNaarRechts();
    void veranderRichting();
    
    void setControleerSensoren(boolean controleren);
  private:
    void controleerSensorenEnVoerActiesUit();
    void verwijderAlleSensorChecks();
    AccelStepper* motorRechts;
    AccelStepper* motorLinks;
    MultiStepper* motoren;
    boolean controleerSensoren;
    int richting;
    int standaardSnelheid;
    SensorActie sensorActies[MAXSENSORCHECKS];
    int stappenPerKeer;
};
#endif