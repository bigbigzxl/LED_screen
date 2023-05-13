#include "display.h"

#define INLINE inline __attribute__((always_inline))
#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#define _width 47
#define _height 7

// class Display
// {

// };


CRGB* _canvasBuffer = (CRGB *)malloc(sizeof(CRGB) * _width * _height);
uint16_t* _matrixIndex =  (uint16_t* )malloc(sizeof(uint16_t) * _width * _height);

uint32_t dotOrder(uint16_t x, uint16_t y)
{
    uint16_t factor = floor(x / 5);
    uint32_t order = y * 5 + x % 5 + factor * 35;
    return order;
}

uint16_t XY(uint16_t x, uint16_t y)
{
    return _matrixIndex[y * _width + x];
}

void init_index(void)
{
    for (int x = 0; x < _width; x++)
        for (int y = 0; y < _height; y++)
        {
            _matrixIndex[y * _width + x] = dotOrder(x, y);
        }
}

void writePixel(int16_t x, int16_t y, const CRGB& color)
{
    if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

    _canvasBuffer[XY(x,y)] = color;
    
}

void drawChar(int16_t x, int16_t y, unsigned char c, CRGB& color,CRGB& bg)
{
    // 'Classic' built-in font
    if((x >= _width)            || // Clip right
        (y >= _height)          || // Clip bottom
        ((x + 6 * 1 - 1) < 0)      || // Clip left
        ((y + 8 * 1 - 1) < 0))        // Clip top
        return;
    
    if((c >= 176)) c++; // Handle 'classic' charset behavior

    for(int8_t i=0; i<5; i++ ) { // Char bitmap = 5 columns
        uint8_t line = pgm_read_byte(&font[c * 5 + i]);
        for(int8_t j=0; j<8; j++, line >>= 1) {
            if(line & 1) {
                writePixel(x+i, y+j, color);
            } 
            else if(bg != color) {
                writePixel(x+i, y+j, bg);
            }
        }
    }
    // if(bg != color) { // If opaque, draw vertical line for last column
    //     if(size == 1) writeFastVLine(x+5, y, 8, bg);
    //     else          writeFillRect(x+5*size, y, size, 8*size, bg);
    // }
    
}