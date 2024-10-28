#include "HX711.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DT_PIN  8
#define SCK_PIN 9
#define SERVO1_PIN 10
#define SERVO2_PIN 11
#define SERVO3_PIN 12
#define SERVO4_PIN 13
#define SERVO5_PIN 7
#define IR_SENSOR_PIN 6  // Pin for the IR sensor

HX711 scale;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// LCD configuration (address 0x27 is common for many LCDs, but it can vary)
LiquidCrystal_I2C lcd(0x27, 16, 4);

float calibration_factor = 1600.0; // adjust calibration
int count = 0;  // Variable to hold the count detected by the IR sensor

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);

  // Initialize servos to 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);
  servo5.write(0);

  // Initialize LCD
  lcd.begin();
  lcd.backlight(); // Turn on the backlight

  Serial.println("Calibration");
  Serial.println("Calibrate the desired weight");
  delay(5000);
}

void loop() {
  if (scale.is_ready()) {
    // Read the weight from the scale
    long reading = scale.get_units();

    // Update LCD with weight and calibration information
    lcd.clear(); // Clear previous display content

    lcd.setCursor(0, 0);
    lcd.print("Calib: ");
    lcd.print(calibration_factor);

    lcd.setCursor(0, 1);
    lcd.print("Weight: ");
    lcd.print(reading);
    lcd.print(" g");

    if (reading >= 71 && reading <= 100) {
      servo1.write(60);
      servo2.write(0);
      servo3.write(0);
      servo4.write(0);
      servo5.write(0);
      delay(5000);
    } else if (reading >= 65 && reading <= 70) {
      servo1.write(0);
      servo2.write(45);
      servo3.write(0);
      servo4.write(0);
      servo5.write(0);
      delay(5000);
    } else if (reading >= 60 && reading <= 64) {
      servo1.write(0);
      servo2.write(0);
      servo3.write(60);
      servo4.write(0);
      servo5.write(0);
      delay(5000);
    } else if (reading >= 55 && reading <= 59) {
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
      servo4.write(60);
      servo5.write(0);
      delay(5000);
    } else if (reading >= 45 && reading <= 54) {
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
      servo4.write(0);
      servo5.write(60);
      delay(5000);
    } else {
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
      servo4.write(0);
      servo5.write(0);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scale not detected.");
  }

  delay(1000);
}
