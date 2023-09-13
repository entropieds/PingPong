#ifndef _H_ENGINE_
#define _H_ENGINE_

class engine {
  private:
    uint8_t Y_axe = 0b00010000;
    uint8_t X_axe = 3;
    typedef enum  { west, east, northEast, southEast, southWest, northWest, newDir } direction;
    direction dir = northEast;
  public:
    uint8_t Leftpaddle = 0b00111000;
    uint8_t Rightpaddle = 0b00111000;
    uint8_t acceptAreaLeft = 5;
    uint8_t acceptAreaRight = 5;
    uint8_t pattern[8] = { Leftpaddle, 0, 0, Y_axe, 0, 0, 0,  Rightpaddle};
    void moveBall (uint8_t (&array) [8]);
    void newGame();
};


#endif