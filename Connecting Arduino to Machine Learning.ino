#include <Servo.h>

Servo myServo;  // Create a Servo object

void setup() {
  myServo.attach(5);  // Attach the servo signal pin to GPIO 5 (D1)

  // Start Serial communication
  Serial.begin(9600);

  // Wait for Serial to be ready
  while (!Serial) {
    ; // Wait for serial port to be available
  }

  Serial.println("Ready to receive logs...");
}

void readLogs() {
   String logMessage = "";
  
  // Read all available bytes and accumulate them into logMessage
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();  // Read incoming byte
    logMessage += incomingChar;         // Append the character to logMessage
  }

  // Check if there's any message received
  if (logMessage.length() > 0) {
    Serial.println("Received log: " + logMessage);

    // Process the log message after receiving all characters
    if (logMessage.indexOf("Good") != -1) {
      // Move servo for "Good Egg"
      ResetServo();
      Serial.println("Good Egg detected: Servo moved to 180 degrees.");
    } 
    else if (logMessage.indexOf("Bad") != -1) {
      // Move servo for "Bad Egg" and keep it in position for 5 seconds
      MoveServo(120, 10000);
      ResetServo();
      Serial.println("Bad Egg detected: Servo moved to 135 degrees for 5 seconds, then reset.");
    } else {
      Serial.println("No recognized log message." + logMessage);
      ResetServo();
    }
  }
}

void ResetServo(){
  myServo.write(0);
  delay(500);  
}
// Move the servo to different positions with delay
void MoveServo(int deg1, int delayTime) {
  myServo.write(deg1);    // Move servo to desired position
  delay(delayTime);       // Wait for specified time
}

bool justStarted = true;
void loop() {
  if(justStarted){
    ResetServo();
    justStarted = false;
  }
  // Continuously read logs and process actions
  readLogs();
  // You can add any other logic here, but readLogs() will keep listening for serial input
}
