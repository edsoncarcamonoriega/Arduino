//#######################################################################################################################################################
//#######################################################         DEFINITION OF VARIABLES          ######################################################
//#######################################################################################################################################################


// =====================================
// Define RELAYS FOR SONICATOR
// =====================================
const int sonicator = 3; // X-axis limit switch pin

// =====================================
// Define the buttons for different methods Green=24 Blue=96
// =====================================
#define PROTOCOL_24 47
#define PROTOCOL_96 51
//const int PROTOCOL_24 = 47;
//const int PROTOCOL_96 = 51;
// =====================================
// TB6600 Dual Motor Control (MotorX & MotorY)
// =====================================
// -------- MotorX setup --------
const int stepPinX = 23;   // PUL+ for MotorX
const int dirPinX  = 27;   // DIR+ for MotorX
const int enPinX   = 31;   // ENA+ for MotorX
// -------- MotorY setup --------
const int stepPinY = 35;   // PUL+ for MotorY
const int dirPinY  = 39;   // DIR+ for MotorY
const int enPinY   = 43;   // ENA+ for MotorY

// -------- MotorZ setup --------
const int Zup  = 7;   // DIR+ for MotorY
const int Zdown = 5;   // PUL+ for MotorY

// -------- Limit switch setup --------
const int limitSwitchX = 10; // X-axis limit switch pin
const int limitSwitchY = 12; // Y-axis limit switch pin

// =====================================
// Define the pulses of Y and X
// =====================================

const unsigned long pulsesPerRevX = 800UL;    // 200 steps * 4 microsteps
const float revPerMMX = 1.0f;                 // T6x1 leadscrew = 1 rev = 1 mm
const unsigned long pulsesPerMMX = (unsigned long)(pulsesPerRevX * revPerMMX);

const unsigned long pulsesPerRevY = 800UL;    
const float revPerMMY = 1.0f;                 
const unsigned long pulsesPerMMY = (unsigned long)(pulsesPerRevY * revPerMMY);


const unsigned int travelMMX = 8;           
const unsigned long totalPulsesX = pulsesPerMMX * (unsigned long)travelMMX;
const unsigned int travelMMY = 8;
const unsigned long totalPulsesY = pulsesPerMMY * (unsigned long)travelMMY;


const unsigned int stepPulseHighUsX = 15;
const unsigned int stepDelayUsX     = 50;
const unsigned int stepPulseHighUsY = 100;
const unsigned int stepDelayUsY     = 100;


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
// MotorZ functions
// =====================================



// =====================================
// Homing function for MotorX
// =====================================
void homeMotorX() {

  while (digitalRead(limitSwitchX) == HIGH) { // HIGH = not pressed
    moveMotorX(LOW, (pulsesPerMMX * 1)); 
  }

  Serial.println("Limit switch hit!");

  // Back off 1 mm in HIGH direction
  moveMotorX(HIGH, (pulsesPerMMX * 15));


  Serial.println("Homing complete. MotorX at reference position.");
}

// =====================================
// Homing function for MotorY
// =====================================
void homeMotorY() {
  
  while (digitalRead(limitSwitchY) == HIGH) { // HIGH = not pressed
    moveMotorY(LOW, (pulsesPerMMY * 1));
  } 
  // Back off 5 mm in HIGH direction
  moveMotorY(HIGH, (pulsesPerMMY * 20));
  
}


// =====================================
// Homing function for MotorZ
// =====================================
void homeMotorZ() {
    digitalWrite(Zdown, HIGH);
    delay(10000);
    digitalWrite(Zdown, LOW);
}  

// =====================================
// SONICATOR STEP
// =====================================

void Sonicate(unsigned long time) {
  //moveMotorZ(HIGH, (pulsesPerMMZ * 5.5)); //MOVE Z TO POSITION SONICATION POSITION
  delay(1000);
  digitalWrite(Zup, HIGH);
  delay(10000);
  digitalWrite(Zup, LOW);
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(time); //SONICATION DURATION
  digitalWrite(sonicator, LOW); //STOP SONICATION
  digitalWrite(Zdown, HIGH);
  delay(10000);
  //moveMotorZ(LOW, (pulsesPerMMZ * 5.5)); //MOVE Z LOW POSITION
  }

  // =====================================
// WASH STEP
// =====================================

void Wash() {
  moveMotorX(HIGH, (pulsesPerMMX * 340)); //MOVE TO WASH POSITION 
  //moveMotorZ(HIGH, (pulsesPerMMZ * 5.5)); //MOVE Z TO POSITION SONICATION POSITION
  digitalWrite(Zup, HIGH);
  delay(10000);
  digitalWrite(Zup, LOW);
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(5000); //SONICATION DURATION
  digitalWrite(sonicator, LOW); //STOP SONICATION
  digitalWrite(Zdown, HIGH);
  delay(10000);
  digitalWrite(Zdown, LOW);
  //moveMotorZ(LOW, (pulsesPerMMZ * 4)); //LOWER Z POSITON FOR DRYING THE TIPS
  digitalWrite(sonicator, HIGH); //START SONICATION
  delay(1000); //SONICATION DURATION
  digitalWrite(sonicator, LOW); //STOP SONICATION
  //moveMotorZ(LOW, (pulsesPerMMZ * 1.5)); //MOVE Z TO LOW POSITION  
  moveMotorX(LOW, (pulsesPerMMX * 340)); //MOVE TO SAMPLE PLATE  
  }


//#######################################################################################################################################################
//#######################################################                PROTOCOLS             ##########################################################
//#######################################################################################################################################################

void loop() {
  int PROTOCOL96 = digitalRead(PROTOCOL_96);
  int PROTOCOL24 = digitalRead(PROTOCOL_24);
  //########   1mm = pulsesPerMMX * 4
//  moveMotorX(LOW, (pulsesPerMMX * 340));
//  digitalWrite(Zup, HIGH);
//  delay(10000);
//  digitalWrite(Zup, LOW);
//  delay(500000);
//  digitalWrite(Zdown, HIGH);
//  delay(10000);
//  digitalWrite(Zdown, LOW);
  
  if (PROTOCOL96 == LOW) {
    homeMotorZ(); 
    homeMotorY();
    homeMotorX();

    //Q1
    Sonicate(1000);
    Wash();

    //Q2
    moveMotorY(LOW, (pulsesPerMMY * 36));  
    Sonicate(1000);
    Wash(); 

    //Q3
    moveMotorY(HIGH, (pulsesPerMMY * 36));
    moveMotorX(HIGH, (pulsesPerMMX * 36));

    //Q4
    moveMotorY(LOW, (pulsesPerMMY * 36));  
    Sonicate(1000);
    Wash();   

    disableMotorY();
    disableMotorX();
  }
 // delay(2000);
  //digitalWrite(Zup, LOW);
  
  if (PROTOCOL24 == LOW) {
    homeMotorY();
    homeMotorX();
    homeMotorZ(); 

    moveMotorY(HIGH, (pulsesPerMMY * 18));
    moveMotorX(HIGH, (pulsesPerMMX * 18));
    Sonicate(1000);
    Wash();
  }

}
