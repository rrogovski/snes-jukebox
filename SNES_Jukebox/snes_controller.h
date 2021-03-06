#ifndef NES_CONTROLLER_H
#define NES_CONTROLLER_H

#include <arduino.h>

struct SNESController {
public:
  enum BUTTON {
    A      = 0b0000000010000000,
    X      = 0b0000000001000000,
    L      = 0b0000000000100000,
    R      = 0b0000000000010000,
    B      = 0b1000000000000000,
    Y      = 0b0100000000000000,
    SELECT = 0b0010000000000000,
    START  = 0b0001000000000000,
    UP     = 0b0000100000000000,
    DOWN   = 0b0000010000000000,
    LEFT   = 0b0000001000000000,
    RIGHT  = 0b0000000100000000,
  };

  SNESController(byte latchPin, byte clockPin, byte dataPin) {
    this->latchPin = latchPin;
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    controllerStatus = 0xFFFF;
    lastControllerStatus = 0xFFFF;
    
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT_PULLUP);
    digitalWrite(latchPin, 0);
    digitalWrite(clockPin, 0);
  }
  
  void update() {
    lastControllerStatus = controllerStatus;

    // Latch the data
    digitalWrite(latchPin, 1);
    digitalWrite(latchPin, 0);
    
    // Read the data
    controllerStatus = 0;
    for (int i = 0; i < 16; i++) {
      // Grab a button
      controllerStatus <<= 1;
      controllerStatus += digitalRead(dataPin);
      
      // Clock to the next button
      digitalWrite(clockPin, 1);
      digitalWrite(clockPin, 0);
    }
  }
  
  bool isPressed(BUTTON button) {
    return (controllerStatus & button) == 0;
  }
  
  bool wasPressed(BUTTON button) {
    return (lastControllerStatus & button) == 0;
  }
  
  bool justPressed(BUTTON button) {
    return (lastControllerStatus & button) != 0 && (controllerStatus & button) == 0;
  }
  
  bool justReleased(BUTTON button) {
    return (controllerStatus & button) != 0 && (lastControllerStatus & button) == 0;
  }
  
  word getStatus() { return controllerStatus; }

private:
  byte latchPin;
  byte clockPin;
  byte dataPin;

  word controllerStatus;
  word lastControllerStatus;
};

#endif

