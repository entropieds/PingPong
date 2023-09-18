#ifndef _H_ENGINE_
#define _H_ENGINE_

class engine {
  private:
    uint8_t Y_axe ;
    uint8_t X_axe;
    uint8_t leftScore;
    uint8_t rightScore;
    typedef enum  { northEast, southEast, southWest, northWest, newRound } direction;
    direction dir;
  public:
    uint8_t Leftpaddle;
    uint8_t Rightpaddle;
    uint8_t acceptAreaLeft;
    uint8_t acceptAreaRight;
    engine();
    uint8_t pattern[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
    void moveBall (uint8_t (&array) [8]);
    void newGame();
};


#endif