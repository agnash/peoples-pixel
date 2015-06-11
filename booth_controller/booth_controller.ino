#include <Wire.h>

/*
* temporary - will replace with approriate libaries 7-segment display
*/
#include <LiquidCrystal.h>

// after bit shifting will be converted to I2C slave address 8
#define SLAVE_ADDRESS 0x02

/*
* temporary - initialize lcd object and set I2C simulating arming button 
* pin/state
*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const char armingPin = 6;
char armingButtonState = 0;

// the controller must be armed by an I2C message before the user button will be
// activated. Once armed the system cannot be armed again until after a
// countdown and I2C send
boolean systemArmed = false;

void setup() {
  // start I2C slave
  Wire.begin(SLAVE_ADDRESS);
  
  // I2C communication callbacks
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  /*
  * temporary - setup and test lcd
  */
  lcd.begin(16, 2);
  lcd.print("Online");
  
  /*
  * temporary
  */
  pinMode(armingPin, INPUT);
}

void loop() {
  /*
  * temporary - use a button to simulate receiving an arming message over I2C
  */
  armingButtonState = digitalRead(armingPin);
  if (armingButtonState == HIGH) {
    receiveData(1);
  }
}

void receiveData(int numBytes) {
  if (!systemArmed) {
    systemArmed = true;
    
    /*
    * temporary
    */
    lcd.setCursor(0,0);
    lcd.print("System armed");
  }
}

void sendData() {
  
}
