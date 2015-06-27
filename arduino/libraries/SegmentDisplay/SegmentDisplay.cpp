#include "Arduino.h"
#include "7segment.h"

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
  // prefill the table with error codes
  for (int i = 0; i < PINS; i++) {
    PinMap[i] = -1;
  }
  // map the segments to the appropriate digital pins
  PinMap[seg.E] = pin1;
  PinMap[seg.D] = pin2;
  PinMap[seg.C] = pin3;
  PinMap[seg.G] = pin4;
  PinMap[seg.F] = pin5;
  PinMap[seg.DP] = pin6;
  PinMap[seg.A] = pin7;
  PinMap[seg.B] = pin8;
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
  Node* ptr = table[number];
  clear();
  while (ptr.nextNode != NULL) {
    digitalWrite(PinMap[ptr.segment], HIGH);
    ptr = ptr.nextNode;
  }
}

void SegmentDisplay::clear() const
{
  digitalWrite(PinMap[seg.E], LOW);
  digitalWrite(PinMap[seg.D], LOW);
  digitalWrite(PinMap[seg.C], LOW);
  digitalWrite(PinMap[seg.G], LOW);
  digitalWrite(PinMap[seg.F], LOW);
  digitalWrite(PinMap[seg.DP], LOW);
  digitalWrite(PinMap[seg.A], LOW);
  digitalWrite(PinMap[seg.B], LOW);
}

void SegmentDisplay::buildTable()
{
  table[0] = Node(seg.A, Node(seg.B, Node(seg.C, Node(seg.D, Node(seg.E, Node(seg.F))))));
  table[1] = Node(seg.B, Node(seg.C));
  table[2] = Node(seg.A, Node(seg.B, Node(seg.G, Node(seg.E, Node(seg.D)))));
  table[3] = Node(seg.A, Node(seg.B, Node(seg.G, Node(seg.C, Node(seg.D)))));
  table[4] = Node(seg.F, Node(seg.G, Node(seg.B, Node(seg.C))));
  table[5] = Node(seg.A, Node(seg.F, Node(seg.G, Node(seg.C, Node(seg.D)))));
  table[6] = Node(seg.A, Node(seg.F, Node(seg.G, Node(seg.C, Node(seg.D, Node(seg.E))))));
  table[7] = Node(seg.A, Node(seg.B, Node(seg.C)));
  table[8] = Node(seg.A, Node(seg.B, Node(seg.C, Node(seg.D, Node(seg.E, Node(seg.F, Node(seg.G)))))));
  table[9] = Node(seg.A, Node(seg.B, Node(seg.C, Node(seg.F, Node(seg.G)))));
  table[10] = Node(seg.DP);
  table[11] = Node(seg.A, Node(seg.D, Node(seg.E, Node(seg.F, Node(seg.G)))));
}
