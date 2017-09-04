#include "AfstandsSensor.h" 

AfstandsSensor::AfstandsSensor(int trigPin, int echoPin)
{
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
}

/*
 * Deze functie controleert of er een obstakel voor de sensor staat. 
 * Hij maakt daarvoor gebruik van de functie: meetAfstand(...)
 * Pas wanneer de functie 3 keer is aangeroepen EN hij 3 keer een obstakel heeft gezien geeft hij 'true' terug
 * 
 */ 
boolean AfstandsSensor::controleerObstakelOpDrempelwaarde()
{
  //In de variabele obstakelTeller houden we bij hoe vaak we een obstakel achter elkaar hebben gedetecteerd.
  //Omdat hij static is, wordt de waarde onthouden en alleen de eerste keer op 0 gezet.
  
  //In de variabele afstand onthouden we de gemeten afstand.
  float afstand = meetAfstand();
  if (afstand > 0 && afstand < drempelwaarde)
  {
    //Als de afstand groter dan 0 is en kleiner dan de minimale afstand hebben we een obstakel gedetecteerd.
    //We tellen 1 bij de obstakelTeller op.
    obstakelTeller = obstakelTeller + 1;
  }
  else
  {
    //In alle andere gevallen hebben we geen obstakel gedetecteerd en zetten we de obstakelTeller weer op 0
    obstakelTeller = 0;
  }
  
  if (obstakelTeller > 2)
  {
    //Wanneer we 3 keer een obstakel hebben gedetecteerd (en dus tussendoor niet op 0 is gezet) geven
    //we 'true' terug.
    //Voordat we dat doen zetten we alvast de obstakelTeller op 0 voor de volgende detectie ronde.
    obstakelTeller = 0;
    return true;
  }
  else
  {
    //In alle andere gevallen returnen we 'false'
    return false;
  } 
}

/*
 *  Deze functie leest de afstandssensor uit. Hoe dat moet staat in de datasheet van de sensor.  
 *  De stappen om het aantal centimeters uit te lezen zijn:
 *  1. Start de meting door de pin 'Trig' 10 uS HIGH te maken en dan weer LOW
 *  2. Wacht tot de sensor een puls terugstuurt op de pin 'Echo' en meet hoe lang die is.
 *  3. Deel de lengte van die puls door 58. 
 *  
 *  Let op: om deze functie te kunnen gebruiken moeten je eerst de volgende dingen doen:
 *  1. Maak een globale variabele 'afstandssensorTrigPin' en zet de waarde op de pin waarop je 'Trig' hebt aangesloten
 *  2. Maak een globale variabele 'afstandssensorEchoPin' en zet de waarde op de pin waarop je 'Echo' hebt aangesloten
 *  3. Zet de pinMode van de 'afstandssensorTrigPin' op OUTPUT
 *  3. Zet de pinMode van de 'afstandssensorEchoPin' op INPUT
 */ 
float AfstandsSensor::meetAfstand()
{
   //In echoPulsLengte bewaren we de lengte van de puls.
  long echoPulsLengte = 0;
  //Voor de zekerheid maken we eerst de TrigPin LOW
  digitalWrite(trigPin, LOW);  // Added this line
  //Even wachten
  delayMicroseconds(5); 
  //Stap 1:
  //We maken de TrigPin HIGH
  digitalWrite(trigPin, HIGH);
  //we wachten 10 uS
  delayMicroseconds(10); // Added this line
  //en maken de TrigPin weer LOW
  digitalWrite(trigPin, LOW);
  //Stap 2:
  //De functie puslseIn van Arduino doet precies wat we willen voor stap 2:
  //Hij wacht tot er een puls komt en meet hoe lang die is.
  //We vertellen de functie:
  //op welke pin hij de puls moet meten,
  //dat de puls HIGH is, 
  //en dat hij maximaal 3000uS moet wachten op de puls.
  echoPulsLengte = pulseIn(echoPin, HIGH, 3000);
  //Stap 3: we delen de puls lengte door 58.0 en geven de waarde meteen terug.
  //De .0 is belangrijk omdat C anders de deling afrond op een geheel getal 
  //Dan zouden we geen millimeters kunnen meten.
  return (echoPulsLengte / 58.0); 
}

CheckFunctie AfstandsSensor::voorwerpGedetecteerd()
{
  return (CheckFunctie)&controleerObstakelOpDrempelwaarde;
}