#include <AccelStepper.h>

// Define your pins (Example pin assignments, adjust as necessary)
#define stepPinX 22
#define dirPinX 26
#define enPinX 30
#define motorInterfaceType 1 // For driver boards

// Create an instance of the AccelStepper class for Motor X
AccelStepper stepperX = AccelStepper(motorInterfaceType, stepPinX, dirPinX);

// =====================================
// Setup Function (in your main sketch)
// =====================================
void setup() {
  pinMode(enPinX, OUTPUT);
  disableMotorX(); // Start with motor disabled

  // --- Configure Acceleration Settings ---
  stepperX.setMaxSpeed(10000);        // Set maximum speed in steps/s
  stepperX.setAcceleration(5000);     // Set acceleration rate in steps/s^2

  // Optional: Set the motor to an initial position (e.g., home)
  stepperX.setCurrentPosition(0); 
}

// =====================================
// MotorX functions (Modified)
// =====================================

void enableMotorX()   { digitalWrite(enPinX, LOW);  } // LOW = enable
void disableMotorX()  { digitalWrite(enPinX, HIGH); }

// --- New Function to initiate a move with smooth ramping ---
void moveMotorXSmooth(long targetPosition) {
  enableMotorX();
  // Set the target position (absolute steps from home)
  stepperX.moveTo(targetPosition);
}

// --- Function that MUST be called repeatedly in your loop() ---
void runMotorX() {
  // This function drives the motor and manages acceleration/deceleration
  stepperX.run(); 
  
  // Optional: Automatically disable motor when movement is complete
  if (stepperX.distanceToGo() == 0) {
     // disableMotorX(); 
  }
}

// =====================================
// Example of usage in the main loop()
// =====================================
void loop() {
  // Call this constantly in the loop to allow the motor to move
  runMotorX();

  // Example: If a move is completed, start a new one after a delay
  if (stepperX.distanceToGo() == 0) {
    delay(2000); // Wait 2 seconds
    
    // Move 10000 steps away from current position smoothly
    moveMotorXSmooth(stepperX.currentPosition() + 100000);
  }
}
