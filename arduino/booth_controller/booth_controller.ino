// Copyright (c) 2015 Aaron Nash
//
// This source file is licensed under the terms of the "GPL (v2)" license.
// Please see the file LICENSE.md included in this distribution for licensing
// terms.
//

#include <Wire.h>
#include <SegmentDisplay.h>

// after bit shifting will be converted to I2C slave address 8
#define SLAVE_ADDRESS 0x04

SegmentDisplay segmentDisplay;

// ready indicator led pin
const char readyPin = 4;

// user button pin and state
const char buttonPin = 5;
int buttonState;

// countdown timer = 5 seconds
const long interval = 5000;

// codes that can be received from the Raspberry-Pi
enum ReceiveCodes: byte {REBOOT, ARM, DISARM, TRIGGER};

// codes that can be sent to the Raspberry-Pi
enum States: byte {NANR, AR, ANR, CAP, ERR};

// these variables actually hold the current state and last message
byte state;
byte msg;

void setup() {
  // start I2C slave
  Wire.begin(SLAVE_ADDRESS);

  // I2C communication callbacks
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  // communication codes
  ReceiveCodes receives;
  States sends;

  // setup the common cathode segment display
  segmentDisplay.initialize(6, 7, 8, 9, 10, 11, 12, 13);

  // set the readyPin to OUTPUT
  pinMode(readyPin, OUTPUT);

  // set the buttonPin to INPUT
  pinMode(buttonPin, INPUT);

  // set initial states
  state = NANR;
  buttonState = LOW;
}

void loop() {

  if (msg == REBOOT) {
    // perform software reset...
  }

  if (state == NANR && msg == ARM) {
    state = AR;
    digitalWrite(readyPin, HIGH);
  }

  /*****************************************************************
  so that countdown triggering can happen remotely or via the button
  *****************************************************************/
  if ((buttonState = digitalRead(buttonPin)) == HIGH) {
    msg = TRIGGER;
  }
  /****************************************************************/

  if (state == AR && msg == TRIGGER) {
    state = ANR;
    digitalWrite(readyPin, LOW);
    countdown();
    state = CAP;
  }

  if (msg == DISARM) {
    state = NANR;
  }

}

void receiveData(int numBytes) {
  while (Wire.available()) {
    msg = Wire.read();
  }
}

void sendData() {
  // send the outgoing code
  Wire.write(state);
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
}
