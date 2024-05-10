#include <Arduino.h>
#include "Engine.h"

// Constructor 
// init new class of engine
engine::engine() {
  leftScore = 0;
  rightScore = 0;
  randomSeed(analogRead(A0));
  engine::newGame();
}

// Set new game
// private: no
// return value: non
// parametrs: non
// example:

void engine::newGame() {
   Leftpaddle = 0b00111000;       // LeftPaddle starting possition
   Rightpaddle = 0b00111000;      // RightPaddle starting possition
   Y_axe = 1 << random(1,6);      // Get random starting Y possition of ball
   X_axe = random(2,5);           // Get random starting X possition of ball
   pattern[0] = Leftpaddle;       // Store Leftpaddle to display patter array
   pattern[X_axe] = Y_axe;        // Store Y possition to display patter array
   pattern[7] = Rightpaddle;      // Store Rightpaddle to display patter array
   acceptAreaLeft = 5;            // init accept area for Leftpaddle
   acceptAreaRight = 5;           // init accept area for Rightpaddle
   dir = (direction)random(0,3);  // get random start direction
   newRoundBegin = true;          // set that new game was set
}

// ball physic 
// private: no
// return value: non
// parametrs: array of uint8_t

void engine::moveBall (uint8_t (&array) [8]) {
  newRoundBegin = false; // set flag that new game end
  switch (dir) {
    // North east direction
    case northEast:
    // Move north east 
      if ((Y_axe > 1) && (X_axe < 6)){
        Y_axe >>= 1;
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        dir = northEast;
        break;
      }
    // if ball hit paddle set new direction
      if ((array[X_axe] & ((Rightpaddle) | (Rightpaddle << 1))) && (X_axe == 6)) {
        dir = northWest;
        break;
      }
    // if ball hit end of map set new direction
      if (Y_axe == 1) {
        dir = southEast;
        break;
      }
    // if ball miss paddle set new game
      if ((X_axe == 6)) {
        Y_axe >>= 1;
        array[++X_axe] = (Y_axe) | Rightpaddle;
        array[X_axe - 1] = 0;
        dir = newRound;
        ++rightScore;
        break;
      }
      break;
    // Move ball to nortWest
    case northWest:
    // Move ball northWest while is in accept area
      if ((Y_axe > 1) && (X_axe > 1)) {
        Y_axe >>= 1;
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        dir = northWest;
        break;
      }
    // if ball hit paddle set dir to northEast
      if ((array[X_axe] & (Leftpaddle | (Leftpaddle << 1))) && X_axe == 1) {
        dir = northEast;
        break;
      }
    // if ball hit end of display set dir to southEast
      if (Y_axe == 1) {
        dir = southWest;
        break;
      }
    // if ball miss paddle set new round  
      if (X_axe == 1) {
        Y_axe >>= 1;
        array[--X_axe] = (Y_axe) | Leftpaddle;
        array[X_axe + 1] = 0;
        dir = newRound;
        ++leftScore;
        break;
      }
      break;
    // move ball to southEast
    case southEast:
    // Move ball southEast in accpet area
      if ((Y_axe < 128) && (X_axe < 6)){
        Y_axe <<= 1;
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        dir = southEast;
        break;
      }
    // if ball hit paddle set new dir
      if ((array[X_axe] & (Rightpaddle | (Rightpaddle >> 1))) && X_axe == 6) {
        dir = southWest;
        break;
      }
    // if ball hit end of display set new dir
      if (Y_axe == 128) {
        dir = northEast;
        break;
      }
    // if ball miss paddle set new round
      if (X_axe == 6) {
        Y_axe <<= 1;
        array[++X_axe] = (Y_axe) | Rightpaddle;
        array[X_axe - 1] = 0;
        dir = newRound;
        ++rightScore;
        break;
      }
      break;
    // Move ball to southWest
    case southWest:
    // Move ball southWest in accpet area
      if ((Y_axe < 128) && (X_axe > 1)){
        Y_axe <<= 1;
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        dir = southWest;
        break;
      }
    // if ball hit paddle set new dir
      if ((array[X_axe] & (Leftpaddle | (Leftpaddle >> 1))) && X_axe == 1) {
        dir = southEast;
        break;
      }
    // if ball hit end of display set new dir
      if (Y_axe == 128) {
        dir = northWest;
        break;
      }
    // if ball miss paddle set new round
      if (X_axe == 1) {
        Y_axe <<= 1;
        array[--X_axe] = (Y_axe) | Leftpaddle;
        array[X_axe + 1] = 0;
        dir = newRound;
        ++leftScore;
        break;        
      }
      break;
    // set new round
    case newRound:
    // Print players score
      Serial.print("Left Player: ");
      Serial.println(leftScore);
      Serial.print("Right Player: ");
      Serial.println(rightScore);
    // init new game
      engine::newGame();
      break;

    default: break;
  }
}