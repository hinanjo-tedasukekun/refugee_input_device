#ifndef SEND_DATA_H
#define SEND_DATA_H

#include "LiquidCrystal.h"

class InputApp;

class SendData {
private:
  InputApp* app_;
  LiquidCrystal* lcd_;

public:
  SendData(InputApp* app, LiquidCrystal* lcd);
  void execute(String leader_id_, int num_of_members);

private:
  void printSending();
  void printRegistered();
  void printUpdated();
  void printError();

  void blinkLed(int pin);
};

#endif