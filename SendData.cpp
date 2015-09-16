#include <Arduino.h>

#include "InputAppConfig.h"
#include "InputApp.h"
#include "TactSwitch.h"
#include "SendData.h"

SendData::SendData(InputApp* app, I2CLiquidCrystal* lcd) :
  app_(app),
  lcd_(lcd),
  sw_reset_(InputAppConfig::PIN_SW_RESET, InputAppConfig::SW_SHORT_PUSH_COUNT)
{
}

void SendData::execute(String leader_id, int num_of_members) {
  sw_reset_.reset();
  printSending();

  String data = leader_id + ',' + num_of_members;
  Serial.println(data);

  while (1) {
    if (sw_reset_.readState() == TactSwitch::SW_PUSHED) {
      // リセットボタンが押されたら何もしない
      return;
    }

    if (Serial.available() > 0) {
      switch (Serial.read()) {
      case 'R':
        printRegistered();
        blinkLed(InputAppConfig::PIN_LED_SUCCESS);

        break;
      case 'U':
        printUpdated();
        blinkLed(InputAppConfig::PIN_LED_SUCCESS);

        break;
      default:
        printError();
        blinkLed(InputAppConfig::PIN_LED_ERROR);

        break;
      }

      delay(WAIT_TIME_AFTER_SEND);
      return;
    }

    delay(10);
  }
}

void SendData::printSending() {
  lcd_->setCursor(0, 1);
  // "ソウシンチュウ"
  lcd_->print("\xBF\xB3\xBC\xDD\xC1\xAD\xB3        ");
}

void SendData::printRegistered() {
  lcd_->setCursor(0, 1);
  // "トウロクシマシタ"
  lcd_->print("\xC4\xB3\xDB\xB8\xBC\xCF\xBC\xC0       ");
}

void SendData::printUpdated() {
  lcd_->setCursor(0, 1);
  // "コウシンシマシタ"
  lcd_->print("\xBA\xB3\xBC\xDD\xBC\xCF\xBC\xC0       ");
}

void SendData::printError() {
  lcd_->setCursor(0, 1);
  // "トウロクデキマセンデシタ"
  lcd_->print("\xC4\xB3\xDB\xB8\xC3\xDE\xB7\xCF\xBE\xDD\xC3\xDE\xBC\xC0 ");
}

void SendData::blinkLed(int pin) {
  for (int i = 0; i < 3; ++i) {
    digitalWrite(pin, LOW);
    delay(150);
    digitalWrite(pin, HIGH);
    delay(150);
  }
}
