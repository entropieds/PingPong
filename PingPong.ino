#include "encoder.h"
#define PIN_A1 2
#define PIN_B1 5
#define PIN_BTN1 4

const uint8_t pinA1 = 2;
const uint8_t pinB1 = 5;
const uint8_t pinBtn1 = 4;

const uint8_t pinA2 = 3;
const uint8_t pinB2 = 7;
const uint8_t pinBtn2 = 6;

const unit8_t paddlePatternSize = 8;

encoder paddleLeft;
encoder paddleRight;

uint8_t paddle = 0b00111000;
uint8_t pattern[] = { paddle, 0, 0, 0, 0, 0, 0,  paddle };


encoder Encoder;
max7219Matrix MAX7219;

void paddleLeft_ISR() { 
  paddleLeft.encoder_flag = true;
}

void paddleRight_ISR() { 
  paddleRight.encoder_flag = true;
}

uint8_t dir1 = 0;
uint8_t dir2 = 0;

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
  MAX7219.send_pattern(pattern,paddlePatternSize);
  Serial.begin(9600);
}

void loop() {
  dir = Encoder.read_encoder(PIN_A1, PIN_B1);
  if (dir == 0){
    Serial.print("encoder count: ");
    Serial.println(count_encoder);
    ++count_encoder;
  } else if (dir == 1){
    Serial.print("encoder count: ");
    Serial.println(count_encoder);
    --count_encoder;
  }
}

