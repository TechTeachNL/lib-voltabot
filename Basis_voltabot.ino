//Bovenaan zet je welke libraries je programma gebruikt
#include <AccelStepper.h>
#include <MultiStepper.h>

//Hieronder stel je de standaard snelheid in
#define SNELHEID 500

//Wordt nog niet gebruikt
#define STAPPEN_PER_RONDJE 4076

//Maximum aantal sensor checks dat dit programma tegelijk kan uitvoeren
#define MAXCHECKS 5

//Het getal 8 is de code voor onze motor in de library die we gebruiken
#define HALFSTEP 8

//Hieronder stel je in hoe je de motordriver op de Arduino hebt aangesloten
//'#define motorRechtsPin1 4' de '1' achter 'motorRechtsPin' betekent 'IN1' op de motordriver. 
//In dit voorbeeld zijn dus IN1 van de motordriver rechts en 'pin 4' op de Arduino met elkaar verbonden.
//Als jij het anders hebt aangesloten kan je getallen hieronder veranderen.
#define motorRechtsPin1  12     // IN1 on the ULN2003 driver 1
#define motorRechtsPin2  11     // IN2 on the ULN2003 driver 1
#define motorRechtsPin3  10     // IN3 on the ULN2003 driver 1
#define motorRechtsPin4  9     // IN4 on the ULN2003 driver 1

#define motorLinksPin1  8     // IN1 on the ULN2003 driver 1
#define motorLinksPin2  7     // IN2 on the ULN2003 driver 1
#define motorLinksPin3  6     // IN3 on the ULN2003 driver 1
#define motorLinksPin4  5     // IN4 on the ULN2003 driver 1


//Hieronder stel je in op welke pins de afstandssensor is aangesloten
#define afstandssensorTrigPin 4
#define afstandssensorEchoPin 3

//Hieronder stel je in op welke pin de buzzer is aangesloten
#define buzzerPin 2

typedef struct td_Afstandssensor
{
  int trigPin;
  int echoPin;
  int drempelwaarde;
  int obstakelTeller;
} Afstandssensor;

typedef struct td_SensorActie
{
  void (*actie)();
  boolean (*sensor)();
} SensorActie;
  
//Hieronder maak je de objecten die je gaat gebruiken in je programma
const int stappen = 100;
//IN1-IN3-IN2-IN4 voor de 28BYJ-48
AccelStepper motorRechts(HALFSTEP, motorRechtsPin1, motorRechtsPin3, motorRechtsPin2, motorRechtsPin4);
AccelStepper motorLinks(HALFSTEP, motorLinksPin1, motorLinksPin3, motorLinksPin2, motorLinksPin4);
SensorActie sensorActies[MAXCHECKS];
Afstandssensor afstandVoor;
MultiStepper VoltaBotWielen;

boolean controleerSensoren;

void setupVoltaBot()
{
  afstandVoor.trigPin = afstandssensorTrigPin;
  afstandVoor.echoPin = afstandssensorEchoPin;
  afstandVoor.obstakelTeller = 0;
  //Met de drempelwaarde stel je in van hoe ver weg de VoltaBot een obstakel moet detecteren. 20 betekent dichterbij dan 20cm.
  afstandVoor.drempelwaarde = 20;
  
  pinMode(afstandVoor.trigPin, OUTPUT);
  pinMode(afstandVoor.echoPin, INPUT);
  
  memset(&sensorActies, 0, sizeof(sensorActies));
  
  VoltaBotWielen.addStepper(motorLinks);
  VoltaBotWielen.addStepper(motorRechts);

  //Met de functie: setMaxSpeed, stel je de maximum snelheid in.
  motorLinks.setMaxSpeed(SNELHEID);
  //Met de functie: setAcceleratiion, stel je in hoe snel hij moet optrekken. '0' betkent meteen draaien op de ingestelde snelheid.
  motorLinks.setAcceleration(0);

  motorRechts.setMaxSpeed(SNELHEID);
  motorRechts.setAcceleration(0);
}

void checkSensors()
{
  boolean (*check)();
  void (*actie)();
 
  for(int i = 0; i < MAXCHECKS ; i++)
  {
    check = sensorActies[i].sensor;
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

void voegSensorCheckToe(int plek, boolean (*check) (), void (*actie)())
{
  SensorActie sa;
  sa.actie = actie;
  sa.sensor = check;
  if(plek < MAXCHECKS)
  {
      sensorActies[plek] = sa;
  }
}

void piepEnOntwijkVoor()
{
  setControleerSensoren(false);
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
  for(int i = 0; i < 20; i++)
  {
    rijAchteruit();
  }
  for(int i = 0; i < 20; i++)
  {
    draaiNaarLinks();
  }
  setControleerSensoren(true);
}

void piep()
{
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
}

//Alle code in de functie 'setup()' wordt 1 keer uitgevoerd als je de Arduino aanzet, of nadat je op de reset knop heb gedrukt.
void setup() 
{
  pinMode(buzzerPin, OUTPUT);
  setupVoltaBot();
}

void setControleerSensoren(boolean aan)
{
  controleerSensoren = aan;
}
//De code in de functie 'loop()' wordt steeds herhaald. Als de laatste regel code is uitgevoerd begint hij weer van voor af aan.
void loop() 
{
  

  setControleerSensoren(true);
  //Met delay laat je je hele programma een tijdje niks doen. Dit is hetzelfde als het blok: 'wacht' in Scratch. 
  //delay(1000) zorgt ervoor dat je programma precies 1 seconde wacht. Met delay(2000) wacht je programma 2 seconde en delay(500) een halve seconde.
  delay(1000);
  voegSensorCheckToe(0, &controleerObstakelVoor, &piep);
  for(int i = 0;i < 20; i++)
  {
    rijAchteruit();
  }
  delay(1000);
  voegSensorCheckToe(0, &controleerObstakelVoor, &piepEnOntwijkVoor);
  for(int i = 0;i < 100; i++)
  {
    rijVooruit();
  }
  delay(1000);
  voegSensorCheckToe(0, &controleerObstakelVoor, &piep);
  for(int i = 0;i < 40; i++)
  {
    draaiNaarRechts();
  }
  delay(1000);
  voegSensorCheckToe(0, &controleerObstakelVoor, &piepEnOntwijkVoor);
  for(int i = 0;i < 100; i++)
  {
    rijVooruit();
  }
}

boolean controleerObstakelVoor()
{
  if(controleerObstakelOpAfstand(afstandVoor))
  {
    return true;
  }
  return false;
}

void ontwijkVoor()
{
  
}
void rijVooruit()
{
  long snelheden[] = {0- stappen, stappen};
  motorLinks.setCurrentPosition(0);
  motorRechts.setCurrentPosition(0);
  motorLinks.setSpeed(0-SNELHEID);
  motorRechts.setSpeed(0-SNELHEID);
  VoltaBotWielen.moveTo(snelheden);
  VoltaBotWielen.runSpeedToPosition();
  if(controleerSensoren)
  {
    checkSensors();
  }
}

void rijAchteruit()
{
  long snelheden[] = {stappen, 0-stappen};
  motorLinks.setCurrentPosition(0);
  motorRechts.setCurrentPosition(0);
  motorLinks.setSpeed(SNELHEID);
  motorRechts.setSpeed(SNELHEID);
  VoltaBotWielen.moveTo(snelheden);
  VoltaBotWielen.runSpeedToPosition();
  if(controleerSensoren)
  {
    checkSensors();
  }
}

void draaiNaarLinks()
{
  long snelheden[] = {stappen, stappen};
  motorLinks.setCurrentPosition(0);
  motorRechts.setCurrentPosition(0);
  motorLinks.setSpeed(SNELHEID / 2);
  motorRechts.setSpeed(0-(SNELHEID / 2));
  VoltaBotWielen.moveTo(snelheden);
  VoltaBotWielen.runSpeedToPosition();
  if(controleerSensoren)
  {
    checkSensors();
  }
}

void draaiNaarRechts()
{
  long snelheden[] = {0-stappen, 0-stappen};
  motorLinks.setCurrentPosition(0);
  motorRechts.setCurrentPosition(0);
  motorLinks.setSpeed(0 - (SNELHEID / 2));
  motorRechts.setSpeed(0 - (SNELHEID / 2));
  VoltaBotWielen.moveTo(snelheden);
  VoltaBotWielen.runSpeedToPosition();
  if(controleerSensoren)
  {
    checkSensors();
  }
}

/*
 * Deze functie controleert of er een obstakel voor de sensor staat. 
 * Hij maakt daarvoor gebruik van de functie: meetAfstand(...)
 * Pas wanneer de functie 3 keer is aangeroepen EN hij 3 keer een obstakel heeft gezien geeft hij 'true' terug
 * 
 */
boolean controleerObstakelOpAfstand(Afstandssensor &sensor)
{
  //In de variabele obstakelTeller houden we bij hoe vaak we een obstakel achter elkaar hebben gedetecteerd.
  //Omdat hij static is, wordt de waarde onthouden en alleen de eerste keer op 0 gezet.
  
  //In de variabele afstand onthouden we de gemeten afstand.
  float afstand = meetAfstand(sensor);
  if (afstand > 0 && afstand < sensor.drempelwaarde)
  {
    //Als de afstand groter dan 0 is en kleiner dan de minimale afstand hebben we een obstakel gedetecteerd.
    //We tellen 1 bij de obstakelTeller op.
    sensor.obstakelTeller = sensor.obstakelTeller + 1;
  }
  else
  {
    //In alle andere gevallen hebben we geen obstakel gedetecteerd en zetten we de obstakelTeller weer op 0
    sensor.obstakelTeller = 0;
  }
  
  if (sensor.obstakelTeller > 2)
  {
    //Wanneer we 3 keer een obstakel hebben gedetecteerd (en dus tussendoor niet op 0 is gezet) geven
    //we 'true' terug.
    //Voordat we dat doen zetten we alvast de obstakelTeller op 0 voor de volgende detectie ronde.
    sensor.obstakelTeller = 0;
    return true;
  }
  else
  {
    //In alle andere gevallen returnen we 'false'
    return false;
  }
}

/**
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
 
float meetAfstand(Afstandssensor sensor)
{
  //In echoPulsLengte bewaren we de lengte van de puls.
  long echoPulsLengte = 0;
  //Voor de zekerheid maken we eerst de TrigPin LOW
  digitalWrite(sensor.trigPin, LOW);  // Added this line
  //Even wachten
  delayMicroseconds(5); 
  //Stap 1:
  //We maken de TrigPin HIGH
  digitalWrite(sensor.trigPin, HIGH);
  //we wachten 10 uS
  delayMicroseconds(10); // Added this line
  //en maken de TrigPin weer LOW
  digitalWrite(sensor.trigPin, LOW);
  //Stap 2:
  //De functie puslseIn van Arduino doet precies wat we willen voor stap 2:
  //Hij wacht tot er een puls komt en meet hoe lang die is.
  //We vertellen de functie:
  //op welke pin hij de puls moet meten,
  //dat de puls HIGH is, 
  //en dat hij maximaal 3000uS moet wachten op de puls.
  echoPulsLengte = pulseIn(sensor.echoPin, HIGH, 3000);
  //Stap 3: we delen de puls lengte door 58.0 en geven de waarde meteen terug.
  //De .0 is belangrijk omdat C anders de deling afrond op een geheel getal 
  //Dan zouden we geen millimeters kunnen meten.
  return (echoPulsLengte / 58.0);
}

