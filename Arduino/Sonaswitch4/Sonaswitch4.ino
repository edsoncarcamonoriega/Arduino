//#######################################################################################################################################################
//#######################################################         DEFINITION OF VARIABLES          ######################################################
//#######################################################################################################################################################


// =====================================
// Define RELAYS FOR SONICATOR
// =====================================
const int sonicator = 14; // X-axis limit switch pin

// =====================================
// Define the buttons for differenhjkjuiio8p899999999999999999999990=t methods Green=24 Blue=96
// =====================================
#define PROTOCOL_24 46
#define PROTOCOL_96 50
//const int PROTOCOL_24 = 46;
//const int PROTOCOL_96 = 50;
// =====================================
// TB6600 Dual Motor Control (MotorX & MotorY)
// =====================================
// -------- MotorX setup --------
const int stepPinZ = 8;   // PUL+ for MotorX
const int dirPinZ  = 7;   // DIR+ for MotorX
const int enPinZ   = 5;   // ENA+ for MotorX
// -------- MotorY setup --------
const int stepPinY = 22;   // PUL+ for MotorY
const int dirPinY  = 26;   // DIR+ for MotorY
const int enPinY   = 30;   // ENA+ for MotorY

// -------- MotorZ setup --------
const int stepPinX = 34;   // PUL+ for MotorZ
const int dirPinX  = 38;   // DIR+ for MotorZ
const int enPinX   = 42;   // ENA+ for MotorZ
const int Zup   = 44;   // ENA+ for MotorZ
const int Zdown   = 44;   // ENA+ for MotorZ


// -------- Limit switch setup --------
const int limitSwitchX = 12; // X-axis limit switch pin
const int limitSwitchY = 10; // Y-axis limit switch pin

// =====================================
// Define the pulses of Y and X
// =====================================

const unsigned long pulsesPerRevX = 800UL;    // 200 steps * 4 microsteps
const float revPerMMX = 1.0f;                 // T6x1 leadscrew = 1 rev = 1 mm
const unsigned long pulsesPerMMX = (unsigned long)(pulsesPerRevX * revPerMMX);

const unsigned long pulsesPerRevY = 800UL;    
const float revPerMMY = 1.0f;                 
const unsigned long pulsesPerMMY = (unsigned long)(pulsesPerRevY * revPerMMY);

const unsigned long pulsesPerRevZ = 800UL;    
const float revPerMMZ = 1.0f;                 
const unsigned long pulsesPerMMZ = (unsigned long)(pulsesPerRevZ * revPerMMZ);



const unsigned int travelMMX = 8;           
const unsigned long totalPulsesX = pulsesPerMMX * (unsigned long)travelMMX;
const unsigned int travelMMY = 8;
const unsigned long totalPulsesY = pulsesPerMMY * (unsigned long)travelMMY;
const unsigned int travelMMZ = 8;
const unsigned long totalPulsesZ = pulsesPerMMZ * (unsigned long)travelMMZ;


const unsigned int stepPulseHighUsX = 3;
const unsigned int stepDelayUsX     = 30;
const unsigned int stepPulseHighUsY = 3;
const unsigned int stepDelayUsY     = 30;
const unsigned int stepPulseHighUsZ = 10;
const unsigned int stepDelayUsZ     = 200;



//#######################################################################################################################################################
//#######################################################         SETUP VARIABLE MODES         ##########################################################
//#######################################################################################################################################################

void setup() {
  Serial.begin(9600);

  // buttons
  pinMode(PROTOCOL_24, INPUT_PULLUP);
  pinMode(PROTOCOL_96, INPUT_PULLUP);

  // MotorX
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(enPinX, OUTPUT);

  // MotorY
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(enPinY, OUTPUT);

  // MotorZ
  pinMode(stepPinZ, OUTPUT);
  pinMode(dirPinZ, OUTPUT);
  pinMode(enPinZ, OUTPUT);
  pinMode(Zup, OUTPUT);
  pinMode(Zdown, OUTPUT);

  // Limit switch
  pinMode(limitSwitchX, INPUT_PULLUP);
  pinMode(limitSwitchY, INPUT_PULLUP);

  enableMotorX();
  enableMotorY();
  
  // Relays for actuator
  pinMode(sonicator, OUTPUT); // put your setup code here, to run once:


  
}

//#######################################################################################################################################################
//#######################################################                FUNCTIONS             ##########################################################
//#######################################################################################################################################################

// =====================================
// MotorX functions
// =====================================
void enableMotorX()   { digitalWrite(enPinX, LOW);  } // LOW = enable
void disableMotorX()  { digitalWrite(enPinX, HIGH); }

void moveMotorX(bool dir, unsigned long pulses) {
  digitalWrite(dirPinX, dir);
  for (unsigned long i = 0; i < pulses; i++) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(stepPulseHighUsX);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(stepDelayUsX);
  }
}

// =====================================
// MotorY functions
// =====================================
void enableMotorY()   { digitalWrite(enPinY, LOW);  }
void disableMotorY()  { digitalWrite(enPinY, HIGH); }

void moveMotorY(bool dir, unsigned long pulses) {
  digitalWrite(dirPinY, dir);
  for (unsigned long i = 0; i < pulses; i++) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(stepPulseHighUsY);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(stepDelayUsY);
  }
}

// =====================================
// MotorZ functions  3.18 mm por steps
// =====================================
void enableMotorZ()   { digitalWrite(enPinZ, LOW);  }
void disableMotorZ()  { digitalWrite(enPinZ, HIGH); }

void moveMotorZ(bool dir, unsigned long pulses) {
  digitalWrite(dirPinZ, dir);
  for (unsigned long i = 0; i < pulses; i++) {
    digitalWrite(stepPinZ, HIGH);
    delayMicroseconds(stepPulseHighUsZ);
    digitalWrite(stepPinZ, LOW);
    delayMicroseconds(stepDelayUsZ);
  }
}

// =====================================
// Homing function for MotorX
// =====================================
void homeMotorX() {
   delay(500);

  while (digitalRead(limitSwitchX) == HIGH) { // HIGH = not pressed
    moveMotorX(LOW, (pulsesPerMMX * 1)); 
  }
   delay(500);

  Serial.println("Limit switch hit!");

  // Back off 1 mm in HIGH direction
  moveMotorX(HIGH, (pulsesPerMMX * 34));
  delay(500);


  Serial.println("Homing complete. MotorX at reference position.");
}

// =====================================
// Homing function for MotorY
// =====================================
void homeMotorY() {
   delay(500);

  while (digitalRead(limitSwitchY) == HIGH) { // HIGH = not pressed
    moveMotorY(LOW, (pulsesPerMMY * 1));
  } 
   delay(500);

  // Back off 5 mm in HIGH direction
  moveMotorY(HIGH, (pulsesPerMMY * 96));
  
}


// =====================================
// Homing function for MotorZ
// =====================================
void homeMotorZ() {
   delay(500);

  while (digitalRead(limitSwitchY) == HIGH) { // HIGH = not pressed
    moveMotorZ(LOW, (pulsesPerMMZ * 1)); 
  }
   delay(500);
  moveMotorZ(HIGH, (pulsesPerMMZ * 1));
  Serial.println("Limit switch hit!");

  


  Serial.println("Homing complete. MotorX at reference position.");
}


// =====================================
// SONICATOR STEP
// =====================================

void Sonicate(unsigned long time) {
  delay(500);
  moveMotorZ(HIGH, (pulsesPerMMZ * 15)); //MOVE UP TO SONICATE 
  delay(500);
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(time); //SONICATION DURATION
  digitalWrite(sonicator, LOW); //STOP SONICATION
  delay(500);
  moveMotorZ(LOW, (pulsesPerMMZ * 5)); //MOVE UP TO SONICATE 
  delay(500);
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(3000); //SONICATION DURATION
  moveMotorZ(LOW, (pulsesPerMMZ * 10)); //MOVE UP TO SONICATE
  delay(500);
  digitalWrite(sonicator, LOW); //STOP SONICATION
  }

  void Sonicate24(unsigned long time) {
  delay(500);
  moveMotorZ(HIGH, (pulsesPerMMZ * 14)); //MOVE UP TO SONICATE 
  delay(500);
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(time); //SONICATION DURATION
  digitalWrite(sonicator, LOW); //STOP SONICATION
  delay(500);
  moveMotorZ(LOW, (pulsesPerMMZ * 5)); //MOVE UP TO SONICATE 
  delay(500);
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(3000); //SONICATION DURATION
  moveMotorZ(LOW, (pulsesPerMMZ * 9)); //MOVE UP TO SONICATE
  delay(500);
  digitalWrite(sonicator, LOW); //STOP SONICATION
  }


//#######################################################################################################################################################
//#######################################################                PROTOCOLS             ##########################################################
//#######################################################################################################################################################

void loop() {
  int PROTOCOL96 = digitalRead(PROTOCOL_96);
  int PROTOCOL24 = digitalRead(PROTOCOL_24);
  //########   1mm = pulsesPerMMX * 4

  
  enableMotorY();
  enableMotorX();
  enableMotorZ();

  const unsigned long short_sonication = 150000; //150000
  const unsigned long long_sonication = 570000; //570000
  
  if (PROTOCOL96 == LOW) {
    homeMotorZ();
    delay(500);
    homeMotorX();
    delay(500);
    homeMotorY();
    delay(500);
    moveMotorZ(HIGH, (pulsesPerMMY * 15));

    //Q1
    Sonicate(short_sonication);

    //Q2
    moveMotorY(LOW, (pulsesPerMMY * 36));  
    Sonicate(short_sonication);

    //Q3
    moveMotorY(HIGH, (pulsesPerMMY * 36));
    delay(500);
    moveMotorX(HIGH, (pulsesPerMMX * 36));
    Sonicate(short_sonication);

    //Q4
    moveMotorY(LOW, (pulsesPerMMY * 36));  
    Sonicate(short_sonication);

    //Q5
    moveMotorX(HIGH, (pulsesPerMMX * 305));
    moveMotorY(HIGH, (pulsesPerMMY * 36));
    Sonicate(short_sonication);

    //Q6
    moveMotorY(LOW, (pulsesPerMMY * 36));  
    Sonicate(short_sonication);

    //Q7
    moveMotorY(HIGH, (pulsesPerMMY * 36));
    delay(500);
    moveMotorX(HIGH, (pulsesPerMMX * 36));
    Sonicate(short_sonication);

    //Q8
    moveMotorY(LOW, (pulsesPerMMY * 36));  
    Sonicate(short_sonication);
    homeMotorZ();
 

    
  }
  
  if (PROTOCOL24 == LOW) {
    homeMotorZ();
    delay(500);
    homeMotorX();
    delay(500);
    homeMotorY();
    delay(500);
    moveMotorZ(HIGH, (pulsesPerMMY * 15));

    moveMotorY(LOW, (pulsesPerMMY * 19));
    delay(500);
    moveMotorX(HIGH, (pulsesPerMMX * 19));
    Sonicate24(long_sonication);

    moveMotorX(HIGH, (pulsesPerMMX * 345));
    Sonicate24(long_sonication);

    homeMotorZ();


    
  }

}
