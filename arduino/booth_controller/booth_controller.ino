#include <Wire.h>
#include <SegmentDisplay.h>

// after bit shifting will be converted to I2C slave address 8
#define SLAVE_ADDRESS 0x04

SegmentDisplay segmentDisplay;

// armed indicator led pin
const char armedPin = 4;

// user trigger button pin and state
const char triggerPin = 5;
char triggerState = LOW;

// the controller must be armed by an I2C message before the user button will be
// activated. Once armed the system cannot be armed again until after a
// countdown and I2C send
boolean systemArmed = false;

// captures the state of trigger button. If false, prevents a button push
// from triggering a countdown. Should be set to true when the system becomes
// armed and then to false again the first time the button is pushed.
boolean triggerReady = false;

// countdown timer = 5 seconds
const long interval = 5000;

// contains the last data received or sent over the i2c bus
int inCode;
int outCode;

void setup() {
  // start I2C slave
  Wire.begin(SLAVE_ADDRESS);
  
  // I2C communication callbacks
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  // setup the common cathode segment display
  segmentDisplay.initialize(6, 7, 8, 9, 10, 11, 12, 13);

  // set the armedPin to OUTPUT
  pinMode(armedPin, OUTPUT);
  
  // set the triggerPin to INPUT
  pinMode(triggerPin, INPUT);
}

void loop() {
  
  triggerState = digitalRead(triggerPin);
  if (triggerState == HIGH && systemArmed && triggerReady) {
    digitalWrite(armedPin, LOW);
    triggerReady = false;
    
    countdown();
  }
}

void receiveData(int numBytes) {
  while (Wire.available()) {
    // capture the data and decide which command it represents
    inCode = Wire.read();
    switch (inCode) {
      case 1:
        armBeforeCountdown();
        break;
      case 2:
        disarmAfterCountdown();
        break;
      default:
        break;
    }
  }
}

void sendData() {
  // send the outgoing code
  Wire.write(outCode);
}

void armBeforeCountdown() {
  if (!systemArmed) {
    systemArmed = true;
    triggerReady = true;
    digitalWrite(armedPin, HIGH);
  }
}

void disarmAfterCountdown() {
  systemArmed = false;
  
  outCode = 2;
}

// nonblocking countdown timer
void countdown() {
  long curr = millis();
  long delta = millis() - curr;
  while ((interval - delta) >= 0) {
    
    segmentDisplay.print((((interval - delta)) / 1000) + 1);
    
    delta = millis() - curr;
  }
  
  segmentDisplay.clear();
  
  outCode = 1;
}

