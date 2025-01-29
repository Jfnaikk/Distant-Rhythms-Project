#include <Servo.h>  // Include the Servo library

const int pulsePin = A0;    // Pin where the pulse sensor is connected (Analog pin A0)
const int ledPin = 13;      // Pin where the LED is connected (built-in LED on Uno)
const int servoPin = 9;     // Pin where the servo is connected (Digital pin 9)

int pulseValue = 0;         // Variable to store the pulse sensor value
int threshold = 550;        // Threshold value to detect pulse (adjust if needed)
int servoPosition = 90;     // Start the servo in the middle position
int lastPulseValue = 0;     // Variable to store the previous pulse value (for filtering)
unsigned long lastPulseTime = 0;  // To track the time since the last pulse detection
unsigned long pulseTimeout = 500; // Timeout period for idle state in milliseconds

Servo myServo;  // Create a Servo object

void setup() {
  pinMode(ledPin, OUTPUT);  // Set the LED pin as an output
  pinMode(pulsePin, INPUT); // Set the pulse sensor pin as an input
  Serial.begin(9600);       // Start serial communication for debugging
  
  myServo.attach(servoPin); // Attach the servo to the specified pin
  myServo.write(servoPosition); // Start the servo at the neutral position
}

void loop() {
  pulseValue = analogRead(pulsePin);  // Read the value from the pulse sensor

  // Print the pulse value to the Serial Monitor for debugging
  Serial.print("Pulse Sensor Value: ");
  Serial.println(pulseValue);

  // Only react to pulse value changes above the threshold and after a short debounce
  if (pulseValue > threshold && (pulseValue > lastPulseValue + 10)) {  // Adjust threshold for noise filtering
    digitalWrite(ledPin, HIGH);  // Turn on the LED when a pulse is detected

    // Map the pulse value to the servo range (0 to 180 degrees)
    servoPosition = map(pulseValue, 550, 1023, 0, 180);
    servoPosition = constrain(servoPosition, 0, 180); // Ensure the servo position stays within 0 to 180 degrees
    myServo.write(servoPosition); // Move the servo to the mapped position

    lastPulseTime = millis();  // Update the time of the last detected pulse
  } else if (millis() - lastPulseTime > pulseTimeout) {
    // If no pulse has been detected for a certain time (idle state), reset the servo to neutral position
    digitalWrite(ledPin, LOW);   // Turn off the LED when no pulse is detected
    myServo.write(90);           // Reset servo to the middle (neutral) position when idle
  }

  lastPulseValue = pulseValue;  // Update last pulse value for filtering purposes

  delay(10);  // Small delay to make the reading stable
}
