#ifndef _ENCODER_H_
#define _ENCODER_H_

class encoder{
  private:
    void debounce(uint8_t pin);
    uint32_t run_time;
    uint32_t old_state;
  public:
    void encoder_init(uint8_t pin_A, uint8_t pin_B, uint8_t pin_btn);
    uint8_t read_encoder(uint8_t pin_A, uint8_t pin_B);
    uint8_t read_button(uint8_t pin_btn);
};
static volatile bool encoder_flag = false;
static volatile bool button_flag = false;
#endif