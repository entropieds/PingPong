#include <Arduino.h>
#include "Engine.h"

void engine::newGame() {
   Leftpaddle = 0b00111000;
   Rightpaddle = 0b00111000;
   Y_axe = 0b00010000;
   X_axe = 3;
   pattern[0] = Leftpaddle;
   pattern[X_axe] = Y_axe;
   pattern[7] = Rightpaddle;
   acceptAreaLeft = 5;
   acceptAreaRight = 5;
   dir = northEast;
}


void engine::moveBall (uint8_t (&array) [8]) {
  switch (dir) {
    case west:
      if (X_axe > 1){
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        dir = west;
        break;
      } 
      if (array[X_axe] & (Leftpaddle)) {
        dir = east;
        break;
      }
      if (X_axe == 1) {
         array[--X_axe] = (Y_axe) | Leftpaddle;
         array[X_axe + 1] = 0;
         dir = newDir;
         break;
      }
      
    case east:
      if (X_axe < 6){
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        dir = east;
        break;
      } 
      if (array[X_axe] & (Rightpaddle)) {
        dir = west;
        break;
      } 
      if (X_axe == 6) {
         array[++X_axe] = (Y_axe) | Rightpaddle;
         array[X_axe - 1] = 0;
         dir = newDir;
         break;
      }

    case northEast:
      if ((Y_axe > 1) && (X_axe < 6)){
        Y_axe >>= 1;
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        break;
      }
      if ((array[X_axe] & (Rightpaddle)) ) {
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
        dir = newDir;
      }
      break;

    case northWest:
      if ((Y_axe > 0b0000001) && (X_axe > 1)) {
        Y_axe >>= 1;
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        break;
      }
      if (array[X_axe] & (Leftpaddle)) {
        dir = southEast;
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
        dir = newDir;
        break;
      }
      break;

    case southEast:
      if ((Y_axe <= 0b1000000) && (X_axe < 6)){
        Y_axe <<= 1;
        array[++X_axe] = (Y_axe);
        array[X_axe - 1] = 0;
        break;
      }
      if (array[X_axe] & (Rightpaddle)) {
        dir = southWest;
        break;
      }
      if (Y_axe == 0b10000000) {
        dir = northEast;
        break;
      }
      if (X_axe == 6) {
        Y_axe <<= 1;
        array[++X_axe] = (Y_axe) | Rightpaddle;
        array[X_axe - 1] = 0;
        dir = newDir;
        break;
      }
      break;

    case southWest:
      if ((Y_axe <= 0b1000000) && (X_axe > 1)){
        Y_axe <<= 1;
        array[--X_axe] = (Y_axe);
        array[X_axe + 1] = 0;
        break;
      }
      if (array[X_axe] & (Leftpaddle)) {
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
        dir = newDir;        
      }
      break;
    
    default: 
      engine::newGame();
      break;
  }
}