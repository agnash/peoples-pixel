// Copyright (c) 2015 Aaron Nash
//
// This source file is licensed under the terms of the "GPL (v2)" license.
// Please see the file LICENSE.md included in this distribution for licensing
// terms.
//

#include "Arduino.h"
#include "SegmentDisplay.h"

SegmentDisplay::SegmentDisplay()
{
  buildTable();
}

SegmentDisplay::SegmentDisplay(
  const byte& pin1,
  const byte& pin2,
  const byte& pin3,
  const byte& pin4,
  const byte& pin5,
  const byte& pin6,
  const byte& pin7,
  const byte& pin8)
{
  initialize(pin1, pin2, pin3, pin4, pin5, pin6, pin7, pin8);
  buildTable();
}

SegmentDisplay::~SegmentDisplay()
{
}

void SegmentDisplay::initialize(
  const byte& pin1,
  const byte& pin2,
  const byte& pin3,
  const byte& pin4,
  const byte& pin5,
  const byte& pin6,
  const byte& pin7,
  const byte& pin8)
{
  // set pin modes
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  // map the segments to the appropriate digital pins
  pinMap[E] = pin1;
  pinMap[D] = pin2;
  pinMap[C] = pin3;
  pinMap[G] = pin4;
  pinMap[F] = pin5;
  pinMap[DP] = pin6;
  pinMap[A] = pin7;
  pinMap[B] = pin8;
}

int SegmentDisplay::print(const byte& number) const
{
  if (number < 0 || number > 9) {
    illuminate(ERROR);
    return -1;
  }
  illuminate(number);
  return number;
}

void SegmentDisplay::illuminate(const byte& number) const
{
  clear();
  byte index = 0;
  byte curr = alphabet[number][index];
  while (curr != 0) {
    digitalWrite(pinMap[curr], HIGH);
    index++;
    curr = alphabet[number][index];
  }
}

void SegmentDisplay::clear() const
{
  digitalWrite(pinMap[E], LOW);
  digitalWrite(pinMap[D], LOW);
  digitalWrite(pinMap[C], LOW);
  digitalWrite(pinMap[G], LOW);
  digitalWrite(pinMap[F], LOW);
  digitalWrite(pinMap[DP], LOW);
  digitalWrite(pinMap[A], LOW);
  digitalWrite(pinMap[B], LOW);
}

void SegmentDisplay::buildTable()
{
  // assignment of character definitions to alphabet table
  alphabet[0] = zero;
  alphabet[1] = one;
  alphabet[2] = two;
  alphabet[3] = three;
  alphabet[4] = four;
  alphabet[5] = five;
  alphabet[6] = six;
  alphabet[7] = seven;
  alphabet[8] = eight;
  alphabet[9] = nine;
  alphabet[DOT] = dot;
  alphabet[ERROR] = error;
}
