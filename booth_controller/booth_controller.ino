#include <Wire.h>

// after bit shifting will be converted to I2C slave address 8
#define SLAVE_ADDRESS 0x02

void setup() {
  // start I2C slave
  Wire.begin(SLAVE_ADDRESS);
  
  // I2C communication callbacks
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop() {
  

}

void receiveData(int numBytes) {

  
}

void sendData() {
  
  
}
