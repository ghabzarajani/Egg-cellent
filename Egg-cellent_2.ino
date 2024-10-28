#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define IR_SENSOR_PIN D8  // GPIO 2 (D8)

// Initialize the LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 4); // Change to 16, 4 for a 16x4 LCD

int eggCount = 0; // Variable to count detected eggs
bool lastState = LOW; // Last state of the sensor
bool isFull = false; // Flag to indicate if the count is full

void setup() {
  Serial.begin(115200); // Initialize Serial Monitor
  pinMode(IR_SENSOR_PIN, INPUT); // Set IR sensor pin as input
  
  lcd.begin(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  
  // Initial display
  lcd.setCursor(5, 0); // First row
  lcd.print("SMALL"); 
  lcd.setCursor(0, 1); // Second row
  lcd.print("EGG COUNT: "); 
  lcd.print(eggCount); // Show egg count on LCD
}

void loop() {
  int sensorValue = digitalRead(IR_SENSOR_PIN); // Read the state of the IR sensor

  // Check for a transition from LOW to HIGH (object detected)
  if (sensorValue == HIGH && lastState == LOW && !isFull) {
    eggCount++; // Increment egg count
    Serial.print("Egg detected: ");
    Serial.println(eggCount); // Print egg count

    // Update LCD display
    lcd.clear(); // Clear the display for a fresh update
    lcd.setCursor(5, 0); // Move cursor to the first row
    lcd.print("SMALL"); 
    lcd.setCursor(0, 1); // Move cursor to the second row
    lcd.print("EGG COUNT: "); 
    lcd.print(eggCount); // Show egg count on LCD

    // Check if the count has reached 10
    if (eggCount >= 10) {
      Serial.println("FULL");
      lcd.clear();
      lcd.print("FULL");
      isFull = true; // Set the full flag
      delay(10000); // Wait for 10 seconds
      eggCount = 0; // Reset count
      Serial.println("Counting reset to 0.");
      isFull = false;
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("SMALL"); // Reset size display
      lcd.setCursor(0, 1);
      lcd.print("EGG COUNT: 0"); // Reset egg count display
    }

    // Add a delay to prevent counting the same egg multiple times
    delay(1000); // Adjust this delay as necessary
  }

  lastState = sensorValue; // Update last state
  delay(100); // Small delay for stability
}
