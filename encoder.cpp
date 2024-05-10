#include <Arduino.h>
#include "encoder.h"

void encoder::encoder_init(uint8_t pin_A, uint8_t pin_B, uint8_t pin_btn) {
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  pinMode(pin_btn, INPUT_PULLUP);
}

uint8_t encoder::read_encoder(uint8_t pin_A, uint8_t pin_B) {
  encoder::debounce(pin_A);
  if (digitalRead(pin_A) != digitalRead(pin_B)) {
    return 0; // clock wise rotation
  } else {
    return 1; // anti clock wise rotation
  } 
}


uint8_t encoder::read_button(uint8_t pin_btn){
  encoder::debounce(pin_btn);
  return digitalRead(pin_btn);
}

void encoder::debounce(uint8_t pin) {
  int8_t current_state = digitalRead(pin);
  uint16_t state_count = 200;
  while(state_count != 0){
    uint8_t new_state = digitalRead(pin);
    if (current_state == new_state){
      --state_count;
    } else {
      ++state_count;
    }
    current_state = new_state;
  }
}