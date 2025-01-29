#include <Servo.h>

// Create a Servo object
Servo myServo;

// Pin configuration
const int pulsePin = A0; // Pulse sensor connected to A0

// Heartbeat motion parameters
int pulseDuration = 200;   // Duration of each pulse in milliseconds
int restDuration = 600;    // Rest between beats in milliseconds
int neutralPosition = 90;  // Neutral position to stop the servo
int forwardPosition = 60;  // Position for contraction
int backwardPosition = 120;// Position for relaxation

// Pulse detection parameters
int baselinePulse = 0;     // Baseline pulse value
int threshold = 30;        // Minimum variance to trigger the servo (adjustable)
int hysteresis = 10;       // Prevents continuous false triggering
int stableReadings = 20;   // Number of stable readings before updating baseline
int stableCounter = 0;     // Counter to track stable readings

// Flag to track whether the servo is currently moving
bool isMoving = false;

void setup() {
  // Attach the servo to pin D2
  myServo.attach(2);
  myServo.write(neutralPosition); // Start at neutral position

  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Establish initial baseline pulse value
  for (int i = 0; i < 10; i++) {
    baselinePulse += analogRead(pulsePin);
    delay(10);
  }
  baselinePulse /= 10;  // Take the average of initial readings
}

void loop() {
  // Read the current pulse value
  int currentPulseValue = analogRead(pulsePin);

  // Calculate the variance from the baseline
  int variance = abs(currentPulseValue - baselinePulse);

  // Debug: Print pulse values and variance
  Serial.print("Pulse: ");
  Serial.print(currentPulseValue);
  Serial.print(" | Variance: ");
  Serial.print(variance);
  Serial.print(" | isMoving: ");
  Serial.println(isMoving);

  // Detect real pulse variance
  if (variance > threshold) {
    if (!isMoving) { // Only trigger if not already moving
      isMoving = true;
      heartbeatMotion();
    }
    stableCounter = 0; // Reset stability counter
  } 
  else if (isMoving && variance < (threshold - hysteresis)) { 
    // Stop movement only if variance falls below threshold-hysteresis
    myServo.write(neutralPosition);
    isMoving = false;
  } 
  else {
    // Increase stability counter when pulse is stable
    stableCounter++;
    
    // Update baseline if stable for a while
    if (stableCounter >= stableReadings) {
      baselinePulse = currentPulseValue;
      stableCounter = 0;
    }
  }

  // Small delay to stabilize readings
  delay(50);
}

// Function to perform heartbeat motion
void heartbeatMotion() {
  // Mimic a heartbeat (1st "lub")
  myServo.write(forwardPosition); // Contraction
  delay(pulseDuration);           // Hold the pulse
  myServo.write(neutralPosition); // Rest
  delay(pulseDuration / 2);       // Short pause

  // Mimic a heartbeat (2nd "dub")
  myServo.write(backwardPosition); // Relaxation
  delay(pulseDuration);            // Hold the pulse
  myServo.write(neutralPosition);  // Rest
  delay(restDuration);             // Longer pause before next beat
}
