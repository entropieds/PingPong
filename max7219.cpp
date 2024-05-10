#include "max7219.h"

#define DECODE_MODE   0x09  // Decode mode addres
#define INTENSITY     0x0A  // Intensity addres
#define SCAN_LIMIT    0x0B  // Scan limit addres
#define SHUTDOWN      0x0C  // Shutdown display addres
#define DISPLAY_TEST  0x0F  // Display test addres

#define MAX7219_LOAD 10 // Pin connected to MAX7219 LOAD/CS pin
#define MAX7219_DIN 11 // Pin connected to MAX7219 DIN pin
#define MAX7219_CLK 13 // Pin connected to MAX7219 CLK pin


// Send command to max7219 driver
// private: yes
// Return value: non
// Parametrs: 
//  addres - unsigned integer that refer to register addres
//  data - unsigned integer of data that are send 
// example:
//  max7219Matrix::send_cmd(0x09,0x00); // set device to no decode mode

void max7219Matrix::send_cmd(uint8_t addres, uint8_t data){
  digitalWrite(MAX7219_LOAD, LOW);  // set cs pin to low state.
  SPI.transfer(addres);             // send register addres 
  SPI.transfer(data);               // send data
  digitalWrite(MAX7219_LOAD, HIGH); // set cs pin to high state
}

// Init display
// private: no
// Return value: non
// Parametrs: non
// example: 
//  max7219Matrix::init_max7219(); // init diplay

void max7219Matrix::init_max7219(){
  pinMode(MAX7219_LOAD, OUTPUT);            // set cs pin as aoutput
  max7219Matrix::setup_decode_mode(0);      // set non decode mode
  max7219Matrix::setup_bright(0x00);        // set brightnes of diplay to lowest value
  max7219Matrix::setup_scan_limit(0x07);    // set scan limit display 
  max7219Matrix::shut_down(false);          // turn on diplay
  max7219Matrix::setup_display_test(0x00);  // turn off test display
}

// Setup brignest of display
// private: no
// return value: non
// parametrs: unsignet integer refer to brightnes of diplay (range from 0x00 to 0x0F)
// example:
//  max7219Matrix::setup_bright(0x0F); // set brigntnes to maximum

void max7219Matrix::setup_bright(uint8_t bright){
  max7219Matrix::send_cmd(INTENSITY, bright);
}

// setup decode mode 
// private: no
// return vlaue: non
// parametrs: unsigned integer that set decode mode (0 - no decode mode, !0 - decode mode)
// example:
//  max7219Matrix::setup_decode_mode(0) // turn off decode mode

void max7219Matrix::setup_decode_mode(uint8_t mode){
  if(mode == 0)
   max7219Matrix::send_cmd(DECODE_MODE, 0x00);  // turn off decode mode
  else 
   max7219Matrix::send_cmd(DECODE_MODE, 0x01);  // turn on decode mode
}

// setup scan limit of display
// private: no
// return value: non
// parametrs: unsigned intiger to set scan limit (rnage from 0x00 to 0x07)
// example:
//  max7219Matrix::setup_scan_limit(0x07); // set scan limit to max

void max7219Matrix::setup_scan_limit(uint8_t scanLimit){
  max7219Matrix::send_cmd(SCAN_LIMIT, scanLimit);
}

// turn on or turn off display
// private: no
// return value: non
// parametrs: unisgnet integer (0 - turn on display, !0 - turn off display)
// example:
//  max7219Matrix::shut_down(0x01); // trun off display

void max7219Matrix::shut_down(bool data){
  if(!data)
    max7219Matrix::send_cmd(SHUTDOWN, 0x01);
  else
    max7219Matrix::send_cmd(SHUTDOWN, 0x00);
}

// setup display test
// private: no
// return value: non
// parametrs: unsigned integer (0 - turn off test display, !0 - turn on test)
// example:
//  max7219Matrix::setup_display_test(0x00); // trun off test

void max7219Matrix::setup_display_test(uint8_t data){
  if(data == 0)
    max7219Matrix::send_cmd(DISPLAY_TEST, 0x00);
  else
    max7219Matrix::send_cmd(DISPLAY_TEST, 0x01);
}

// send display pattern
// private: no
// return value: non
// parametrs: pointer to array, size of array
// example:
//  max7219Matrix::send_pattern(pattern, size); // send pattern to display

void max7219Matrix::send_pattern(uint8_t* pattern, uint8_t size){
  max7219Matrix::setup_scan_limit(size - 1);  // if is pattern smaller than 8 byte 
  for(uint8_t i = 0; i < size; ++i){
      max7219Matrix::select_dot(i + 1, *(pattern + i));
  }
  
}

// trun on dot on display
// private: no
// return value: non
// parametrs: row and collum
// example:
//  max7219Matrix::select_dot(1, 128); // turn on led in frist collum and frist row

void max7219Matrix::select_dot(uint8_t collum, uint8_t row){
  max7219Matrix::send_cmd(collum, row);
}
