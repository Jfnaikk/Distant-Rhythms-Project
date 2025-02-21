#include <Servo.h>

Servo breathingServo;  // Servo for breathing movement
Servo heartServo;      // Servo for heartbeat mimicry

const int BREATHING_SERVO_PIN = 9;  // Breathing servo connected to digital pin 9
const int HEARTBEAT_SERVO_PIN = 3;  // Heartbeat servo connected to digital pin 3
const int PULSE_SENSOR_PIN = A0;    // Pulse sensor connected to analog pin A0

int pulseValue = 0;   // Stores pulse sensor value
const int PULSE_THRESHOLD = 550; // Adjust this based on your pulse sensor readings

void setup() {
  breathingServo.attach(BREATHING_SERVO_PIN);  // Attach breathing servo
  heartServo.attach(HEARTBEAT_SERVO_PIN);      // Attach heartbeat servo
  Serial.begin(115200);  // Initialize serial communication
}

void loop() {
  pulseValue = analogRead(PULSE_SENSOR_PIN); // Read pulse sensor value

  if (pulseValue > PULSE_THRESHOLD) {  // If a pulse is detected
    Serial.println("Pulse Detected ❤️");
    
    // Perform breathing and heartbeat movement
    breathe();
    multipleBeats(6); // Perform 6 beats per pulse
  } else {
    Serial.println("No Pulse ❌");

    // Keep servos still when no pulse detected
    breathingServo.write(90);
    heartServo.write(90);
  }

  delay(100); // Small delay to prevent unnecessary sensor noise
}

void breathe() {
  Serial.println("Breathing 🌬️");
  breathingServo.write(100);  // Move servo slightly up (inhale)
  delay(500);
  breathingServo.write(90);   // Return to neutral position
  delay(500);
  breathingServo.write(80);   // Move servo slightly down (exhale)
  delay(500);
  breathingServo.write(90);   // Return to neutral position
  delay(500);
}

void multipleBeats(int beats) {
  for (int i = 0; i < beats; i++) {
    Serial.println("Heartbeat 💓");
    heartServo.write(100); // Quick contraction
    delay(120);
    heartServo.write(80);  // Quick relaxation
    delay(120);
    heartServo.write(90);  // Neutral position
    delay(250);  // Small pause before next beat
  }
}
