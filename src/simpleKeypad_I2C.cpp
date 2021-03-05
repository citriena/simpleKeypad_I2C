#include <LiquidTWI2.h>
#include <simpleKeypad_I2C.h>


#ifdef MCP23017
// Connect via i2c, address 0x20 (A0-A2 not jumpered)
LiquidTWI2 lcd_keypad(0x20);
#endif


simpleKeypad::simpleKeypad(int16_t interval, int16_t repeatDelay, int16_t readTimes) :
  _REPEAT_INTERVAL(interval),
  _REPEAT_DELAY(repeatDelay),
  _READ_TIMES(readTimes)
{}


simpleKeypad::simpleKeypad(int16_t interval, int16_t repeatDelay) :
  _REPEAT_INTERVAL(interval),
  _REPEAT_DELAY(repeatDelay)
{}


simpleKeypad::simpleKeypad(int16_t interval) :
  _REPEAT_INTERVAL(interval)
{}


simpleKeypad::simpleKeypad() {
}


#ifdef MCP23017
btnCODE_t simpleKeypad::read_button_code() {
  return lcd_keypad.readButtons();
}

#else
btnCODE_t simpleKeypad::read_button_code() {
  btnCODE_t keyCode = btnNONE;

  int16_t adc_key_in = 1023;                          // initial value is that of not key pressed
  int16_t key_in;

  for (int i = 0; i < _READ_TIMES; i++) {         // read _READ_TIMKES times for key bouncing measure
    key_in = analogRead(0);                       // read the value from the sensor
    if (key_in < adc_key_in) adc_key_in = key_in; // use smallest value to avoid key chattering fault
  }
// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
// we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) {
    return btnNONE;         // We make this the 1st option for speed reasons since it will be the most likely result
  }
// キー押し続けた場合の処理のため、すぐにreturnしない。このため判定順序を入れ替えた。
  else if (adc_key_in > 700) keyCode = btnSELECT; //741
  else if (adc_key_in > 450) keyCode = btnLEFT;   //504
  else if (adc_key_in > 300) keyCode = btnDOWN;   //329
  else if (adc_key_in > 100) keyCode = btnUP;     //144
  else keyCode = btnRIGHT;  // 0

  return keyCode;
}
#endif


btnCODE_t simpleKeypad::read_buttons() {

  static unsigned long prevKeyTime = 0;
  unsigned long tKeyTime;
  static int16_t repeatTimes = 0;
  static btnCODE_t prevKeyCode = btnNONE;  // 前回押されていたキー
  btnCODE_t keyCode = btnNONE;             // 押されているキー

  keyCode = read_button_code();
  if (keyCode == btnNONE) {
    prevKeyCode = btnNONE;
    return btnNONE;
  }

  if (keyCode == prevKeyCode) {
    tKeyTime = millis() - _REPEAT_INTERVAL; // 
    if (tKeyTime < prevKeyTime) {           // millis() で直接比較するとなぜか想定通りに動作しない
      return btnVOID;                       // btnBOID means key pressed continuously but not elapsed _REPEAT_INTERVAL ms from previous key input.
    }
    if (repeatTimes == 0) {
      tKeyTime = millis() - _REPEAT_DELAY;
      if (tKeyTime < prevKeyTime) {
        return btnVOID;
      }
    }
    repeatTimes++;
  } else {
    prevKeyCode = keyCode;
    repeatTimes = 0;
  }
  prevKeyTime = millis();
  return keyCode;
}


void simpleKeypad::repeatInterval(int16_t interval) {
  _REPEAT_INTERVAL = interval;
}
