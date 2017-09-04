#include "VoltaBot.h"

VoltaBot::VoltaBot(int linksIn1, int linksIn2, int linksIn3, int linksIn4, int rechtsIn1, int rechtsIn2, int rechtsIn3, int rechtsIn4)
{
  motorLinks = new AccelStepper(HALFSTEP, linksIn1, linksIn2, linksIn3, linksIn4);
  motorRechts = new AccelStepper(HALFSTEP, rechtsIn1, rechtsIn2, rechtsIn3, rechtsIn4);
  motoren = new MultiStepper();
  motoren->addStepper(*motorLinks);
  motoren->addStepper(*motorRechts);
  standaardSnelheid = 200;
  richting = VOORUIT;
  controleerSensoren = false;
  verwijderAlleSensorChecks();
  stappenPerKeer = 100;
}

void VoltaBot::setupVoltaBot()
{
  motorLinks->setMaxSpeed(standaardSnelheid);
  motorRechts->setMaxSpeed(standaardSnelheid);
  motorLinks->setAcceleration(0);
  motorRechts->setAcceleration(0);
  
}

void VoltaBot::setStandaardSnelheid(int snelheid)
{
  standaardSnelheid = snelheid;
}

void VoltaBot::voegSensorCheckToe(int plek, boolean (*check) (), void (*actie)())
{
  if(plek < MAXSENSORCHECKS)
  {
    sensorActies[plek].check = check;
    sensorActies[plek].actie = actie;
  } 
}

void VoltaBot::rij()
{
  if(richting == VOORUIT)
  {
    rijVooruit();
  }
  else if(richting == ACHTERUIT)
  {
    rijAchteruit();
  } 
}

void VoltaBot::rijVooruit()
{
  long afstanden[] = {0 - stappenPerKeer, stappenPerKeer};
  motorLinks->setCurrentPosition(0);
  motorRechts->setCurrentPosition(0);
  motorLinks->setSpeed(standaardSnelheid);
  motorRechts->setSpeed(standaardSnelheid);
  motoren->moveTo(afstanden);
  motoren->runSpeedToPosition();
  if(controleerSensoren)
  {
    controleerSensorenEnVoerActiesUit();
  } 
}

void VoltaBot::rijAchteruit()
{
  long afstanden[] = {stappenPerKeer, 0 - stappenPerKeer};
  motorLinks->setCurrentPosition(0);
  motorRechts->setCurrentPosition(0);
  motorLinks->setSpeed(standaardSnelheid);
  motorRechts->setSpeed(standaardSnelheid);
  motoren->moveTo(afstanden);
  motoren->runSpeedToPosition();
  if(controleerSensoren)
  {
    controleerSensorenEnVoerActiesUit();
  } 
}

void VoltaBot::draaiNaarLinks()
{
 long afstanden[] = {stappenPerKeer, stappenPerKeer};
  motorLinks->setCurrentPosition(0);
  motorRechts->setCurrentPosition(0);
  motorLinks->setSpeed(standaardSnelheid);
  motorRechts->setSpeed(standaardSnelheid);
  motoren->moveTo(afstanden);
  motoren->runSpeedToPosition();
  if(controleerSensoren)
  {
    controleerSensorenEnVoerActiesUit();
  } 
}


void VoltaBot::draaiNaarRechts()
{
   long afstanden[] = {0 - stappenPerKeer, 0 - stappenPerKeer};
  motorLinks->setCurrentPosition(0);
  motorRechts->setCurrentPosition(0);
  motorLinks->setSpeed(standaardSnelheid);
  motorRechts->setSpeed(standaardSnelheid);
  motoren->moveTo(afstanden);
  motoren->runSpeedToPosition();
  if(controleerSensoren)
  {
    controleerSensorenEnVoerActiesUit();
  } 
}


void VoltaBot::veranderRichting()
{
  if(richting == VOORUIT)
  {
    richting = ACHTERUIT;
  }
  else if (richting == ACHTERUIT)
  {
    richting = VOORUIT;
  }  
}

void VoltaBot::setControleerSensoren(boolean controleren)
{
  controleerSensoren = controleren;
}

/*-----------------------------------------------*/
/*Private*/
void VoltaBot::verwijderAlleSensorChecks()
{
  for(int i = 0; i < MAXSENSORCHECKS ; i++)
  {
    sensorActies[i].check = 0;
    sensorActies[i].actie = 0;
  } 
}

void VoltaBot::controleerSensorenEnVoerActiesUit()
{
  boolean (*check)();
  void (*actie)();
 
  for(int i = 0; i < MAXSENSORCHECKS ; i++)
  {
    check = sensorActies[i].check;
    actie = sensorActies[i].actie;

    if(check != 0)
    {
      if(check())
      {
        if(actie != 0)
        {
          actie();
        }
      }
    }
  } 
}
