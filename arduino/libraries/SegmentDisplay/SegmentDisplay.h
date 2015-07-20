// Copyright (c) 2015 Aaron Nash
//
// This source file is licensed under the terms of the "GPL (v2)" license.
// Please see the file LICENSE.md included in this distribution for licensing
// terms.
//

#ifndef SegmentDisplay_h
#define SegmentDisplay_h

#include "Arduino.h"

const byte MAX_DIGITS = 12;
const byte MAX_PINS = 10;
const byte DOT = 10;
const byte ERROR = 11;

class SegmentDisplay {
public:
  // CONSTRUCTORS
  // default constructor
  SegmentDisplay();
  // constructor - only supports common cathode part nos. 18011Ax at the moment
  SegmentDisplay(
    const byte& pin1,
    const byte& pin2,
    const byte& pin3,
    const byte& pin4,
    const byte& pin5,
    const byte& pin6,
    const byte& pin7,
    const byte& pin8
  );

  // DESTRUCTOR
  ~SegmentDisplay();

  // PUBLIC MEMBER FUNCTIONS
  // for manually initializing an existing 7segment instance
  void initialize(
    const byte& pin1,
    const byte& pin2,
    const byte& pin3,
    const byte& pin4,
    const byte& pin5,
    const byte& pin6,
    const byte& pin7,
    const byte& pin8
  );
  // print a number 0-9
  int print(const byte& number) const;
  // clear the display
  void clear() const;

private:
  // PRIVATE MEMBER FUNCTIONS
  // creates the character table
  void buildTable();
  // illuminates a digit given a table index
  void illuminate(const byte& number) const;

  // PRIVATE FIELDS
  // matches a particular LED segment to a particular annode
  // (for now requires 7-segment common cathode part nos. 18011Ax)
  enum seg: byte {E=2, D=3, C=4, G=6, F=7, DP=8, A=9, B=10};
  // seg field
  seg segments;
  // maps the provided digital Arduino pins to the data pins of the specified
  // display
  byte pinMap[MAX_PINS + 1] = {};
  // table of character combinations
  byte* alphabet[MAX_DIGITS];
  // individual character combinations
  byte zero[7] = {A, B, C, D, E, F, 0};
  byte one[3] = {B, C, 0};
  byte two[6] = {A, B, D, E, G, 0};
  byte three[6] = {A, B, C, D, G, 0};
  byte four[5] = {B, C, F, G, 0};
  byte five[6] = {A, C, D, F, G, 0};
  byte six[7] = {A, C, D, E, F, G, 0};
  byte seven[4] = {A, B, C, 0};
  byte eight[8] = {A, B, C, D, E, F, G, 0};
  byte nine[6] = {A, B, C, F, G, 0};
  byte dot[2] = {DP, 0};
  byte error[6] = {A, D, E, F, G, 0};
};

#endif
