#ifndef 7segment_h
#define 7segment_h

#include "Arduino.h"

const byte DIGITS = 12;
const byte PINS = 10;
const byte DOT = 10;
const byte ERROR = 11;

class 7segment {
public:
  // CONSTRUCTORS
  // default constructor
  7segment();
  // constructor - only supports common cathode part nos. 18011Ax at the moment
  7segment(
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
  ~7segment();

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
  enum seg: byte {E=2, D=3, C=4, G=6, F=7, DP=8, A=9, B=10}
  // maps the provided digital Arduino pins to the data pins of the specified
  // display
  int PinMap[PINS];
  // a digit/character is represented as a linked list of annodes stored in a
  // hash table
  struct Node {
    // the annode pin nos.
    int segment;
    // pointer to next annode
    Node* nextNode;
    // constructor
    Node(const int& segment, nextNode = NULL) {
      this.segment = segment;
      this.nextNode = nextNode;
    }
  }
  // table of Node lists
  Node* table[DIGITS];
};

#endif
