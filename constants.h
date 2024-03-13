#define STICK_CP2_ID 4   // unit ID 1 - 4

#if STICK_CP2_ID == 1
  #define GPIO_THM 25
  #define LCD_BRIGHTNESS 48
#elif STICK_CP2_ID == 2
  #define GPIO_THM 25
  #define LCD_BRIGHTNESS 36
#elif STICK_CP2_ID == 3
  #define GPIO_THM 33
  #define LCD_BRIGHTNESS 48
#elif STICK_CP2_ID == 4
  #define GPIO_THM 33
  #define LCD_BRIGHTNESS 32
#endif

#define NUM_ESP32 4

// resistances on ESP32UNIT_ID /k_ohm                 1     2     3     4
const float series_resistance0[NUM_ESP32 + 1] = {0.0, 4.67, 4.68, 4.67, 4.40};

// calibration of each microcomputer + thermosensor /degree
const float calibration[NUM_ESP32 + 1][4] = {{0.0, 0.0, 0.0, 0.0},
    {15.3, 16.1, 26.1, 27.3},   // unit 1  (x1, y1), (x2, y2) Linear function for calibration
    {14.3, 13.1, 25.9, 25.8},   // unit 2
    {10.0, 10.0, 20.0, 20.0},   // unit 3
    {15.0, 13.0, 27.5, 23.2}};  // unit 4

// cSpell:ignore THRMOSENS