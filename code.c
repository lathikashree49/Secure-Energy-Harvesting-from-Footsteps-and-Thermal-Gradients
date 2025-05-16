#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 if 0x3F doesn't work

int i = 0;
char personid[12];
int count = 0;

void setup() {
  pinMode(A0, OUTPUT);  // Use A0 as relay pin
  pinMode(A3, INPUT);   // Input for analog read (sensor)
  pinMode(13, OUTPUT);  // Output for LED indicator
  digitalWrite(A0, HIGH);  // Turn relay off initially (A0 controls the relay)
  Serial.begin(9600);
  lcd.init();             // Initialize the LCD
  lcd.backlight();        // Turn on the backlight
  lcd.clear();            // Clear the display
  lcd.setCursor(0, 0);
  lcd.print("RFID MOBILE     ");
  lcd.setCursor(0, 1);
  lcd.print("CHARGING        ");
  delay(3000);
  lcd.clear();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SCAN THE CARD   ");
  lcd.setCursor(0, 1);
  lcd.print("CHARGE UR PHONE ");
  
  if (Serial.available() > 0) {
    for (i = 0; i < 12; i++) {
      personid[i] = Serial.read();
    }
    personid[i] = '\0';
    
    // Compare the read RFID tag with stored IDs
    if (strncmp(personid, "400031F453D6", 12) == 0) {
      activateCharging("SEENU");
    }
  }

  // Check if the sensor value is high and control the indicator LED
  if (analogRead(A3) > 0) {  // Reading sensor value from A3
    digitalWrite(13, HIGH);  // Turn on the LED when the sensor is active
  } else {
    digitalWrite(13, LOW);   // Turn off the LED
  }
  
  delay(100);  // Delay for stability
}

// Function to handle the charging activation logic
void activateCharging(const char* name) {
  digitalWrite(A0, LOW);  // Turn on the relay (A0 pin)
  for (int i = 60; i >= 0; i--) {
    if (analogRead(A3) > 0) {  // Reading sensor value from A3
      digitalWrite(13, HIGH);  // Turn on the LED when the sensor is active
    } else {
      digitalWrite(13, LOW);   // Turn off the LED
    }
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(name);  // Print the name of the person based on the RFID
    lcd.setCursor(0, 1);
    lcd.print(i);     // Countdown timer
    
    delay(1000);  // 1-second delay for countdown
  }
  
  // After the countdown, turn off the relay and display timeout
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TIME OUT        ");
  digitalWrite(A0, HIGH);  // Turn off the relay
  delay(2000);
}