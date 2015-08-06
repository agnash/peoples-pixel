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

// ready indicator led pin and last state
const char readyPin = 4;
int lastReadyState;

// user button pin and state
const char buttonPin = 5;
int buttonState;

// countdown timer = 5 seconds
const long interval = 5000;

// codes that can be received from the Raspberry-Pi
enum Commands: byte {RESET = 0, ARM = 1, DISARM = 2, TRIGGER = 3, FAULT = 4};
Commands receives;

// codes that can be sent to the Raspberry-Pi
enum States: byte {NANR = 0, AR = 1, ANR = 2, CAP = 3, ERR = 4};
States sends;

// current state and last message
byte state;
byte msg;

void setup() {
  // start I2C slave
  Wire.begin(SLAVE_ADDRESS);

  // I2C communication callbacks
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);



  // setup the common cathode segment display
  segmentDisplay.initialize(6, 7, 8, 9, 10, 11, 12, 13);

  // set the readyPin to OUTPUT
  pinMode(readyPin, OUTPUT);

  // set the buttonPin to INPUT
  pinMode(buttonPin, INPUT);

  // set initial states
  buttonState = LOW;
  digitalWrite(readyPin, LOW);
  segmentDisplay.clear();
  state = NANR;
  msg = DISARM;
}

void loop() {

  if (msg == RESET) {
    setup();
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

  if (msg == FAULT) {
    state = ERR;
  }

  if (state == ERR) {
    segmentDisplay.print(ERROR);
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
