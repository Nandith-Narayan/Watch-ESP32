#ifndef COLOR_DEFINITIONS
#define COLOR_DEFINITIONS

#include <Arduino.h>
#include <TFT_eSPI.h>

extern uint16_t cmap[16];

enum COLOR {
  BLACK = 0,
  RED,
  DARK_GREY,
  LIGHT_GREY,
  WHITE = 15
};

#endif