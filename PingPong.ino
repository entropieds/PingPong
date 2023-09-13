#include "encoder.h"
#include "max7219.h"
#include "Engine.h"


const uint8_t maxVal = 5; 
const uint8_t minVal = 0;


const uint8_t pinA1 = 2;
const uint8_t pinB1 = 5;
const uint8_t pinBtn1 = 4;

const uint8_t pinA2 = 3;
const uint8_t pinB2 = 7;
const uint8_t pinBtn2 = 6;

const uint8_t paddlePatternSize = 8;

encoder paddleLeft;
encoder paddleRight;
max7219Matrix MAX7219;
engine Engine;

void paddleLeft_ISR() { 
  paddleLeft.encoder_flag = true;
}

void paddleRight_ISR() { 
  paddleRight.encoder_flag = true;
}

uint8_t Leftdir = 0;
uint8_t Rightdir = 0;

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
  MAX7219.send_pattern(Engine.pattern,paddlePatternSize);
  Serial.begin(9600);
}



void loop() {
  static uint32_t startTime = millis();
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
  
  if (millis() - startTime >= 750) {
    startTime = millis();
    Engine.moveBall(Engine.pattern);
  }

  MAX7219.send_pattern(Engine.pattern,paddlePatternSize);
  
}

/*
direction getRandomDirection() {
  uint8_t randomValue = random(minVal,maxVal);
  return (direction)(randomValue);
}*/

/*
bool evaluatePoss(uint8_t (&array) [8], uint8_t X_poss) {
return false;
}*/
