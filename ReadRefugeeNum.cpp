#include <Arduino.h>
#include <limits.h>
#include "I2CLiquidCrystal.h"
#include "RingBuffer8.h"
#include "Led.h"
#include "TactSwitch.h"
#include "DeviceSet.h"
#include "InputApp.h"
#include "ReadRefugeeNum.h"

// コンストラクタ
ReadRefugeeNum::ReadRefugeeNum(InputApp* app, DeviceSet* devices) :
  ring_buffer_(),
  ms_end_(ULONG_MAX),
  app_(app),
  devices_(devices)
{
}

// 設定を行う
void ReadRefugeeNum::doSetup() {
  ring_buffer_.clear();

  devices_->reader.turnOn();
  devices_->xbee.sleep();
  devices_->led_success.turnOff();
  devices_->led_error.turnOff();
  devices_->resetAllSwitches();

  // シリアル通信の読み残しがあれば、すべて読んで残りデータを空にする
  while (devices_->reader_serial.available() > 0) {
    devices_->reader_serial.read();
  }

  devices_->lcd.setCursor(0, 0);
  // "バンゴウヲ"
  devices_->lcd.print(F("\xCA\xDE\xDD\xBA\xDE\xB3\xA6         "));

  devices_->lcd.setCursor(0, 1);
  // "ヨミトッテクダサイ"
  devices_->lcd.print(F("\xD6\xD0\xC4\xAF\xC3\xB8\xC0\xDE\xBB\xB2      "));

  ms_end_ = millis() + TIME_LIMIT;
}

// メインループ
void ReadRefugeeNum::doLoop() {
  if (millis() > ms_end_) {
    // 制限時間を過ぎた場合、節電のため強制終了する
    app_->reset();
    return;
  }

  if (devices_->sw_reset.readState() == TactSwitch::SW_PUSHED) {
    app_->reset();
    return;
  }

  if (devices_->reader_serial.available() > 0) {
    char c = devices_->reader_serial.read();

    if (c == '\r') {
      app_->setRefugeeNum(ring_buffer_.toString());
      app_->finishReadRefugeeNum();
      return;
    }

    ring_buffer_.pushBack(c);
  }
}
