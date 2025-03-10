#include "HX711.h"
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DT_PIN  6
#define SCK_PIN 7
#define SERVO1_PIN 10
#define SERVO2_PIN 11
#define SERVO3_PIN 12
#define SERVO4_PIN 13
#define SERVO5_PIN 5

HX711 scale;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

// LCD configuration
LiquidCrystal_I2C lcd1(0x27, 16, 4);  // First LCD (0x27, 16x4)

float calibration_factor = 1605.0; // adjust calibration
float baseline_weight = 0.0; // Store baseline weight
const int numReadings = 10; // Number of readings for smoothing

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare(); // Set initial tare weight
  baseline_weight = getStableWeight(); // Store baseline weight

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
  lcd1.begin(16, 4);
  lcd1.backlight();
  lcd1.setCursor(0, 0);
  lcd1.print("Calibrating...");
  delay(1000);
}

float getStableWeight() {
  float sum = 0;
  for (int i = 0; i < numReadings; i++) {
    sum += scale.get_units();
    delay(50); // Small delay to allow stable readings
  }
  return sum / numReadings; // Return the average
}

void loop() {
  if (scale.is_ready()) {
    float current_weight = getStableWeight();
    float weight_gain = current_weight - baseline_weight; // Calculate weight gain

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Calib: ");
    lcd1.print(calibration_factor);

    lcd1.setCursor(0, 1);
    lcd1.print("Gain: ");
    lcd1.print(weight_gain, 2);
    lcd1.print(" g");

    // Control servos based on weight gain
    if (weight_gain >= 70.0 && weight_gain <= 100.0) {
      servo1.write(60);
      delay(10000);
    } else if (weight_gain >= 65.0 && weight_gain <= 69.9) {
      servo2.write(45);
       delay(10000);
    } else if (weight_gain >= 60.0 && weight_gain <= 64.9) {
      servo3.write(60);
       delay(10000);
    } else if (weight_gain >= 55.0 && weight_gain <= 59.9) {
      servo4.write(55);
       delay(10000);
    } else if (weight_gain >= 50.0 && weight_gain <= 54.9) {
      servo5.write(55);
       delay(10000);
    } else {
      // Reset servos if weight gain is out of range
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
      servo4.write(0);
      servo5.write(0);
    }
  } else {
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print("Scale not detected.");
  }

  delay(1000);
}
