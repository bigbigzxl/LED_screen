#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <math.h>
#include "FastLED.h"
#include "glcdfont.h"
// #include "cmd/cmd.h"

#define INLINE inline __attribute__((always_inline))
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

// namespace SCRREN {

class Display
{

public:
    static Display* getInstance();
    // Display();
    Display(const uint8_t led_pin, const int W, const int H);
    ~Display();

    void showLogo(uint8_t style);

    void setBright(uint8_t s);
    uint16_t dotOrder(uint16_t x, uint16_t y);
    uint16_t XY(uint16_t x, uint16_t y);
    void init_index(void);
    void writePixel(int16_t x, int16_t y, const CRGB& color);
    void drawChar(int16_t x, int16_t y, unsigned char c, CRGB& color,CRGB& bg);
    void drawstring(unsigned char* s);
    void drawstring_slide_in(unsigned char* s);

    void setCharColor(uint8_t r, uint8_t g, uint8_t b);
    void setBgColor(uint8_t r, uint8_t g, uint8_t b);
    // for clear screen;
    void fillScreen(const CRGB& color);
    void screenReset(void);

    // for backgroud
    void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB& color);
    void writeFastVLine(int16_t x, int16_t y, int16_t h, const CRGB& color);

    // void fadeInAll(void);
    void fadeOutAll(uint8_t delta);
    void fadeOutChar(int8_t char_index, uint8_t delta);
    void animation(float *a, float *a_trg, uint8_t n);
    void fadeInString(unsigned char* s, uint8_t delta = 10);

    INLINE void render()
    {
        FastLED.show();
    }
    CRGB* get_buffer(void)
    {
        return m_canvasBuffer;
    }
    
private:
    CRGB* m_canvasBuffer; // = (CRGB *)malloc(sizeof(CRGB) * _width * _height);
    CRGB* m_canvasBuffer_ping; // for animation.
    CRGB* m_canvasBuffer_pong; // for animation.
    uint16_t* m_matrixIndex; // =  (uint16_t* )malloc(sizeof(uint16_t) * _width * _height);
    int m_width;
    int m_height;
    uint16_t m_total_char_num;
    uint8_t  m_ledpin;
    CRGB m_char_color;
    CRGB m_bg_color;

    uint8_t last_fade_delta; // for fade in;

};

// }

#endif