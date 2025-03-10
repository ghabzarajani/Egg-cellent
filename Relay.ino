#include <Servo.h>

#define RELAY_PIN D0  // Relay pin (D0)
#define LED1_PIN D1   // First LED (turns on in step 4)
#define LED2_PIN D2   // Second LED (turns on in step 2)
#define LED3_PIN D3   // Third LED (turns on in step 3)
#define SERVO1_PIN D4 // First Servo motor pin
#define SERVO2_PIN D5 // Second Servo motor pin
#define SERVO3_PIN D8 // Third Servo motor pin

Servo myServo1;
Servo myServo2;
Servo myServo3;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  
  myServo1.attach(SERVO1_PIN);
  myServo2.attach(SERVO2_PIN);
  myServo3.attach(SERVO3_PIN);
  
  myServo1.write(0); // Set first servo to initial position
  myServo2.write(0); // Set second servo to initial position
  myServo3.write(0); // Set third servo to initial position

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

void loop() {
  // Step 1: Relay OFF (1.5 sec) -> All LEDs ON, Activate Servo2, Activate Servo3
  digitalWrite(RELAY_PIN, LOW);
  myServo2.write(60); // Move second servo
  myServo3.write(90); // Move third servo
  delay(1300);
  
  // Step 2: Relay ON (3 sec) -> LED2 ON, Reset Servo2, Keep Servo3 Activated
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED3_PIN, LOW);
  myServo2.write(0); // Reset second servo
  myServo3.write(90); // Keep third servo activated
  delay(3000);

  // Step 3: Relay OFF (5 sec) -> LED3 ON, Keep Servo3 Activated
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, HIGH);
  myServo3.write(90); // Keep third servo activated
  delay(10000);

  // Step 4: Relay ON (4 sec) -> LED1 ON, Move Servo1, Reset Servo3
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  myServo1.write(80); // Move first servo
  myServo2.write(0);
  myServo3.write(0); // Reset third servo to 0 degrees
  delay(4000);

  // Reset first servo position after step 4
  myServo1.write(0);
}
