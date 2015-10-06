#ifndef INPUT_NUM_OF_MEMBERS_H
#define INPUT_NUM_OF_MEMBERS_H

#include "I2CLiquidCrystal.h"

#include "TactSwitch.h"

class InputApp;

class InputNumOfMembers {
public:
  // 扱うスイッチの数
  static const int NUM_OF_SWITCHES = 8;

private:
  int num_of_members_;
  bool num_of_members_changed_;

  InputApp* app_;
  // LCD
  I2CLiquidCrystal* lcd_;

  TactSwitch sw_1_;
  TactSwitch sw_2_;
  TactSwitch sw_3_;
  TactSwitch sw_4_;
  TactSwitch sw_plus_;
  TactSwitch sw_minus_;
  TactSwitch sw_send_;
  TactSwitch sw_reset_;

  TactSwitch* switches_[NUM_OF_SWITCHES];

public:
  // コンストラクタ
  InputNumOfMembers(InputApp* app, I2CLiquidCrystal* lcd);
  // 状態をリセットする
  void reset();
  // IO ポートを初期状態にする
  void setupPorts();
  // LCD を初期状態にする
  void setupLcd();
  // メインループ
  void loop();
  void handleSwitchEvents();
  void updateNumOnLcd();
  void setNumOfMembers(int num);
};

#endif
