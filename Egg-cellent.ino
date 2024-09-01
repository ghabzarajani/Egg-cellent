#include "HX711.h"
#include <Servo.h>

#define DT_PIN  8
#define SCK_PIN 9
#define SERVO_PIN 10

HX711 scale;
Servo myServo;

float calibration_factor = 1750.0; // adjust calibration

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN); 
  scale.set_scale(calibration_factor);
  scale.tare();

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  Serial.println("Calibration");
  Serial.println("Calibrate the desired weight");
  delay(5000);
}

void loop() {
  if (scale.is_ready()) {
    // Read the weight from the scale
    long reading = scale.get_units();
    Serial.print("Weight: ");
    Serial.print(reading);
    Serial.println(" g");

    if (reading >= 65 && reading <= 70) {
      myServo.write(80); 
      delay(5000);
    } else {
      myServo.write(0); 
      
    }
  } else {
    Serial.println("Scale not detected.");
  }
  
  delay(1000);
}
