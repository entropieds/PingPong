#include "encoder.h"
#include "max7219.h"
#include "Engine.h"

// Pins for left encoder
const uint8_t pinA1 = 2;
const uint8_t pinB1 = 5;
const uint8_t pinBtn1 = 4;
// Pins for right encoder
const uint8_t pinA2 = 3;
const uint8_t pinB2 = 7;
const uint8_t pinBtn2 = 6;

encoder paddleLeft;
encoder paddleRight;
max7219Matrix MAX7219;
engine Engine;

// ISR for left encoder
void paddleLeft_ISR() { 
  paddleLeft.encoder_flag = true;
}
// ISR for right encoder
void paddleRight_ISR() { 
  paddleRight.encoder_flag = true;
}

uint8_t Leftdir = 0;    // Check dir of left dir
uint8_t Rightdir = 0;   // Check dir of right dir


void playersReady(uint8_t btnP1, uint8_t btnP2);  // funcion for wait players

void setup() {
  paddleLeft.encoder_init(pinA1, pinB1, pinBtn1);
  paddleRight.encoder_init(pinA2, pinB2, pinBtn2);
  attachInterrupt(digitalPinToInterrupt(pinA2), paddleRight_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinA1), paddleLeft_ISR, CHANGE);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  MAX7219.init_max7219();
  MAX7219.setup_bright(0X0A);
  playersReady(pinBtn1, pinBtn2);
  Serial.begin(9600);
}



void loop() {
  static uint32_t startTime = millis();
  static uint16_t speedUp = 500;
  
  if (Engine.newRoundBegin) {
    speedUp = 500;
    Engine.newRoundBegin = false;
  }

  if (millis() < startTime) {
    startTime = millis();
  }

  if (paddleLeft.encoder_flag) {
    Leftdir = paddleLeft.read_encoder(pinA1, pinB1);
    paddleLeft.encoder_flag = false;
    cli();
    if (!Leftdir && (Engine.acceptAreaLeft > 2)) {
      Engine.Leftpaddle >>= 1;
      --Engine.acceptAreaLeft;
    } else if (Leftdir && (Engine.acceptAreaLeft < 7)){
      Engine.Leftpaddle <<= 1;
      ++Engine.acceptAreaLeft;
    }
    Engine.pattern[0] = Engine.Leftpaddle;
    sei();
  }

  if (paddleRight.encoder_flag) {
    Rightdir = paddleRight.read_encoder(pinA2, pinB2);
    paddleRight.encoder_flag = false;
    cli();
    if (!Rightdir && (Engine.acceptAreaRight > 2)) {
      Engine.Rightpaddle >>= 1;
      --Engine.acceptAreaRight;
    } else if (Rightdir && (Engine.acceptAreaRight < 7)){
      Engine.Rightpaddle <<= 1;
      ++Engine.acceptAreaRight;
    }
    Engine.pattern[7] = Engine.Rightpaddle;
    sei(); 
  }

  if (millis() - startTime >= speedUp) {
    startTime = millis();
    Engine.moveBall(Engine.pattern);
    if (speedUp > 200)
      --speedUp;
    else
      speedUp = 200;
  }
  MAX7219.send_pattern(Engine.pattern,8);
}



void playersReady(uint8_t btnP1, uint8_t btnP2) {
  bool P1 = true, P2 = true;
  uint8_t array[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t array2[8] = { Engine.Leftpaddle, 0, 0, 0, 0, 0, 0, Engine.Rightpaddle};
  MAX7219.send_pattern(array, 8);
  static uint32_t waitTime = millis();
  bool blink = false;
  while (P1 || P2){
    if (!paddleLeft.read_button(btnP1)) {
      array[0] = Engine.Leftpaddle;
      P1 = false;
    }
    if (!paddleRight.read_button(btnP2)){
      array[7] = Engine.Rightpaddle;
      P2 = false;
    }
    if (millis() - waitTime >= 500) {
      waitTime = millis();
      blink = !blink;
      if (blink) {
      MAX7219.send_pattern(array, 8);
      } else {
        MAX7219.send_pattern(array2, 8);
      }
    }
  }
  MAX7219.send_pattern(Engine.pattern,8);
}

