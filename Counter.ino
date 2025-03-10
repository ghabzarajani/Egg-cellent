#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define Ultrasonic Sensor Pins
#define TRIG1_PIN 5
#define ECHO1_PIN 18
#define TRIG2_PIN 19
#define ECHO2_PIN 23
#define TRIG3_PIN 32
#define ECHO3_PIN 33
#define TRIG4_PIN 25
#define ECHO4_PIN 26
#define TRIG5_PIN 27
#define ECHO5_PIN 14
#define TRIG6_PIN 12
#define ECHO6_PIN 13

// Define LCD Addresses
#define I2C_ADDR1 0x27  // First LCD Address
#define I2C_ADDR2 0x25  // Second LCD Address
#define I2C_ADDR3 0x23  // Third LCD Address
#define I2C_ADDR4 0x24  // Fourth LCD Address
#define I2C_ADDR5 0x21  // Fifth LCD Address
#define I2C_ADDR6 0x22  // Sixth LCD Address

#define MAX_COUNT 10    // Max count before reset

// Initialize six LCDs (16x2)
LiquidCrystal_I2C lcd1(I2C_ADDR1, 16, 2);
LiquidCrystal_I2C lcd2(I2C_ADDR2, 16, 2);
LiquidCrystal_I2C lcd3(I2C_ADDR3, 16, 2);
LiquidCrystal_I2C lcd4(I2C_ADDR4, 16, 2);
LiquidCrystal_I2C lcd5(I2C_ADDR5, 16, 2);
LiquidCrystal_I2C lcd6(I2C_ADDR6, 16, 2);

int eggCount1 = 0, eggCount2 = 0, eggCount3 = 0, eggCount4 = 0, eggCount5 = 0, eggCount6 = 0;
float prevDistance1 = 0, prevDistance2 = 0, prevDistance3 = 0, prevDistance4 = 0, prevDistance5 = 0, prevDistance6 = 0;

void setup() {
    Serial.begin(9600);
    pinMode(TRIG1_PIN, OUTPUT); pinMode(ECHO1_PIN, INPUT);
    pinMode(TRIG2_PIN, OUTPUT); pinMode(ECHO2_PIN, INPUT);
    pinMode(TRIG3_PIN, OUTPUT); pinMode(ECHO3_PIN, INPUT);
    pinMode(TRIG4_PIN, OUTPUT); pinMode(ECHO4_PIN, INPUT);
    pinMode(TRIG5_PIN, OUTPUT); pinMode(ECHO5_PIN, INPUT);
    pinMode(TRIG6_PIN, OUTPUT); pinMode(ECHO6_PIN, INPUT);
    initializeLCD(lcd1, "Peewee");
    initializeLCD(lcd2, "Small");
    initializeLCD(lcd3, "Medium");
    initializeLCD(lcd4, "Large");
    initializeLCD(lcd5, "Extra Large");
    initializeLCD(lcd6, "Jumbo");
}

void loop() {
    detectAndUpdate(TRIG1_PIN, ECHO1_PIN, lcd1, eggCount1, prevDistance1);
    detectAndUpdate(TRIG2_PIN, ECHO2_PIN, lcd2, eggCount2, prevDistance2);
    detectAndUpdate(TRIG3_PIN, ECHO3_PIN, lcd3, eggCount3, prevDistance3);
    detectAndUpdate(TRIG4_PIN, ECHO4_PIN, lcd4, eggCount4, prevDistance4);
    detectAndUpdate(TRIG5_PIN, ECHO5_PIN, lcd5, eggCount5, prevDistance5);
    detectAndUpdate(TRIG6_PIN, ECHO6_PIN, lcd6, eggCount6, prevDistance6);
    delay(50);
}

void detectAndUpdate(int trigPin, int echoPin, LiquidCrystal_I2C &lcd, int &eggCount, float &prevDistance) {
    long duration;
    float distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 20000);
    if (duration == 0) return;
    distance = (duration * 0.0343) / 2;
    Serial.print("Sensor"); Serial.print(trigPin); Serial.print(" Distance: "); Serial.println(distance);
    if (distance > 0 && abs(distance - prevDistance) >= 1) { // Detect any change
        eggCount++;
        prevDistance = distance;
        lcd.setCursor(0, 1);
        lcd.print("        ");
        lcd.setCursor(0, 1);
        lcd.print(eggCount);
    }
    if (eggCount >= MAX_COUNT) {
        lcd.setCursor(0, 1);
        lcd.print("FULL!");
        Serial.println("FULL! Resetting in 5s...");
        delay(5000);
        eggCount = 0;
        lcd.setCursor(0, 1);
        lcd.print("        ");
        lcd.setCursor(0, 1);
        lcd.print(eggCount);
    }
}

void initializeLCD(LiquidCrystal_I2C &lcd, const char* title) {
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(title);
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(title);
}
