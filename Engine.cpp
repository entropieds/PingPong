#include <Arduino.h>
#include "Engine.h"

engine::engine() {
  leftScore = 0;
  rightScore = 0;
  randomSeed(analogRead(A0));
  engine::newGame();
}

void engine::newGame() {
   Leftpaddle = 0b00111000;
   Rightpaddle = 0b00111000;
   Y_axe = 1 << random(1,6);
   X_axe = random(1,6);
   pattern[0] = Leftpaddle;
   pattern[X_axe] = Y_axe;
   pattern[7] = Rightpaddle;
   acceptAreaLeft = 5;
   acceptAreaRight = 5;
   dir = (direction)random(0,3);
}


void engine::moveBall (uint8_t (&array) [8]) {
  switch (dir) {
    case northEast:
      if ((Y_axe > 1) && (X_axe < 6)){
        Y_axe >>= 1;
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        dir = northEast;
        break;
      }
      if ((array[X_axe] & (Rightpaddle)) && X_axe == 6) {
        dir = northWest;
        break;
      }
      if (Y_axe == 1) {
        dir = southEast;
        break;
      }
      if (X_axe == 6) {
        Y_axe >>= 1;
        array[++X_axe] = (Y_axe) | Rightpaddle;
        array[X_axe - 1] = 0;
        dir = newRound;
        ++rightScore;
        break;
      }
      break;

    case northWest:
      if ((Y_axe > 1) && (X_axe > 1)) {
        Y_axe >>= 1;
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        dir = northWest;
        break;
      }
      if ((array[X_axe] & (Leftpaddle)) && X_axe == 1) {
        dir = northEast;
        break;
      }
      if (Y_axe == 1) {
        dir = southWest;
        break;
      }
      if (X_axe == 1) {
        Y_axe >>= 1;
        array[--X_axe] = (Y_axe) | Leftpaddle;
        array[X_axe + 1] = 0;
        dir = newRound;
        ++leftScore;
        break;
      }
      break;

    case southEast:
      if ((Y_axe < 128) && (X_axe < 6)){
        Y_axe <<= 1;
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        dir = southEast;
        break;
      }
      if ((array[X_axe] & (Rightpaddle)) && X_axe == 6) {
        dir = southWest;
        break;
      }
      if (Y_axe == 128) {
        dir = northEast;
        break;
      }
      if (X_axe == 6) {
        Y_axe <<= 1;
        array[++X_axe] = (Y_axe) | Rightpaddle;
        array[X_axe - 1] = 0;
        dir = newRound;
        ++rightScore;
        break;
      }
      break;

    case southWest:
      if ((Y_axe < 128) && (X_axe > 1)){
        Y_axe <<= 1;
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        dir = southWest;
        break;
      }
      if ((array[X_axe] & (Leftpaddle)) && X_axe == 1) {
        dir = southEast;
        break;
      }
      if (Y_axe == 128) {
        dir = northWest;
        break;
      }
      if (X_axe == 1) {
        Y_axe <<= 1;
        array[--X_axe] = (Y_axe) | Leftpaddle;
        array[X_axe + 1] = 0;
        dir = newRound;
        ++leftScore;
        break;        
      }
      break;

    case newRound:
      Serial.print("Left Player: ");
      Serial.println(leftScore);
      Serial.print("Right Player: ");
      Serial.println(rightScore);
      engine::newGame();
      break;

    default: break;
  }
}