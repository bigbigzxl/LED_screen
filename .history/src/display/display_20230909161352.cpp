#include "display.h"

// namespace SCRREN {
static Display* g_screen = nullptr;

Display* Display::getInstance()
{
    if (g_screen == nullptr)
    {
        g_screen = new Display(18, 40, 7);
    }
    return g_screen;
}


Display::Display(const uint8_t led_pin, const int W, const int H) : 
m_ledpin(led_pin), 
m_width(W), 
m_height(H),
m_total_char_num(8), 
m_char_color( CRGB(g_char_r,g_char_g,g_char_b)), 
m_bg_color(CRGB(0,0,0)) // CRGB(0,30,0)
{
    m_canvasBuffer_ping = (CRGB *)malloc(sizeof(CRGB) * m_width * m_height);
    m_canvasBuffer_pong = (CRGB *)malloc(sizeof(CRGB) * m_width * m_height);

    m_canvasBuffer = m_canvasBuffer_ping;
    m_matrixIndex  = (uint16_t* )malloc(sizeof(uint16_t) * m_width * m_height);

    FastLED.addLeds<WS2812Controller800Khz, 18, GRB>((CRGB *)m_canvasBuffer, 280).setCorrection(0xFFB0F0);  // Use this for WS2812
    FastLED.setBrightness(10);
    init_index();
}

Display::~Display()
{
    delete m_canvasBuffer;
    delete m_matrixIndex;
}

void Display::setCharColor(uint8_t r, uint8_t g, uint8_t b)
{
    m_char_color = CRGB(r,g,b);
}

void Display::setBgColor(uint8_t r, uint8_t g, uint8_t b)
{
    m_bg_color = CRGB(r,g,b);
}

void Display::showLogo(uint8_t style = 0)
{
    drawstring((unsigned char *)" HIBIKI ");
    
    if (style == 0)
    {
        uint16_t pix_pos[4] = {273,274, 279,278};

        uint32_t i = 0;
        unsigned long start = millis(); // current (relative) time in msecs.
        unsigned long end = start;
        while(end - start < 5000)
        {   
            fadeOutChar(7, 150);
            i++;
            i %= 4;
            m_canvasBuffer[pix_pos[i]] = CRGB::Red;
            render();
            delay(300);
            end = millis(); // current (relative) time in msecs.
        }

        // i = 20;
        // while(i--)
        // {
        //     fadeOutAll(4);
        //     render();
        //     delay(10);
        // }
    }
    else if(style == 1)
    {
         uint16_t pix_pos[18] = {241,242,243,249,254,
                                259,264,269,274,278,
                                277,276,270,265,260,
                                255,250,245};

        uint32_t i = 0;
        unsigned long start = millis(); // current (relative) time in msecs.
        unsigned long end = start;
        while(end - start < 5000)
        {   
            fadeOutChar(7, 240);
            i++;
            i %= 4;
            m_canvasBuffer[pix_pos[i]] = CRGB::Red;
            render();
            delay(10);
            end = millis(); // current (relative) time in msecs.
        }
    }
}
void Display::setBright(uint8_t s)
{
    FastLED.setBrightness(s);
}

void Display::fillScreen(const CRGB& color)
{
    if(m_canvasBuffer) {
        if(color.r == color.g && color.r == color.b) {
            memset(m_canvasBuffer, color.r, m_width * m_height * 3);
        } else {
            uint32_t i, pixels = m_width * m_height;
            for(i=0; i<pixels; i++) m_canvasBuffer[i] = color;
        }
    }
}

void Display::screenReset(void)
{
    fillScreen(CRGB(0,0,0));
    // FastLED.clear();
}


uint16_t Display::dotOrder(uint16_t x, uint16_t y)
{
    uint16_t factor = floor(x / 5);
    uint16_t order = y * 5 + x % 5 + factor * 35;
    return order;
}

uint16_t Display::XY(uint16_t x, uint16_t y)
{
    return m_matrixIndex[y * m_width + x];
}

void Display::init_index(void)
{
    for (int x = 0; x < m_width; x++)
        for (int y = 0; y < m_height; y++)
        {
            m_matrixIndex[y * m_width + x] = dotOrder(x, y);
        }
}

void Display::writePixel(int16_t x, int16_t y, const CRGB& color)
{
    if((x < 0) || (y < 0) || (x >= m_width) || (y >= m_height)) return;

    m_canvasBuffer[XY(x,y)] = color;
    
}

void Display::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const CRGB& color)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }
    
    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }
    
    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);
    
    int16_t err = dx / 2;
    int16_t ystep;
    
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    
    for (; x0<=x1; x0++) {
        if (steep) {
            writePixel(y0, x0, color);
        } else {
            writePixel(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void Display::writeFastVLine(int16_t x, int16_t y, int16_t h, const CRGB& color)
{
    writeLine(x, y, x, y+h-1, color);
}

void Display::drawChar(int16_t x, int16_t y, unsigned char c, CRGB& color,CRGB& bg)
{
    // 'Classic' built-in font
    if((x >= m_width)            || // Clip right
        (y >= m_height)          || // Clip bottom
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
                uint8_t h =  y + j;
                uint8_t w =  x + j;
                if ((w == 0 || h == 0 || w == 39 || h == 6)
                    // (w == 0 && h == 1) ||
                    // (w == 1 && h == 0) ||
                    // (w == 0 && h == 5) ||
                    // (w == 0 && h == 6) ||
                    // (w == 1 && h == 6) ||
                    // (w == 38 && h == 0) ||
                    // (w == 38 && h == 6) ||
                    // (w == 39 && h == 0) ||
                    // (w == 39 && h == 1) ||
                    // (w == 39 && h == 5) ||
                    // (w == 39 && h == 6) 
                    )
                {
                    continue;
                }
                writePixel(x+i, y+j, bg);
            }
        }
    }
}

void Display::drawstring(unsigned char* s)
{
    screenReset();
    
    for (uint16_t x = 0; x < m_width; x++)
    {
        drawChar(x*5, 0, s[x], m_char_color, m_bg_color);
    }
}

void Display::fadeOutAll(uint8_t delta) // default = 10
{ 
    // for(int i = 0; i < m_width*m_height; i++)
    // {
    //     m_canvasBuffer[i].nscale8(250); 
    // }
    fadeToBlackBy(m_canvasBuffer, m_width*m_height, delta); // 200ms 10 -> 4% 25 cycles.
    // fadeLightBy(m_canvasBuffer, m_width*m_height, 50);
    // int pos = random16( m_width*m_height);
    // m_canvasBuffer[m_matrixIndex[pos]] += CHSV( 50 + random8(64), 200, 255);
    // int FRAMES_PER_SECOND = 30;
    // // FastLED.show();
    // FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void Display::fadeOutChar(int8_t char_index, uint8_t delta = 10)
{
    if (char_index < 0)
    {
        while (char_index < 0)
        {
            char_index += m_total_char_num;
        }        
    }

    char_index %= m_total_char_num;
    uint16_t per_char_pixel_num = 5*7;
    fadeToBlackBy(m_canvasBuffer + per_char_pixel_num * char_index, per_char_pixel_num, delta);
    // fadeToBlackBy(m_canvasBuffer, m_width*m_height, 10); 
} 

void Display::animation(float *a, float *a_trg, uint8_t n = 60)
{
  if (fabs(*a - *a_trg) < 0.15) *a = *a_trg;
  if (*a != *a_trg) *a += (*a_trg - *a) / (n / 10.0);
}

void Display::drawstring_slide_in(unsigned char* s)
{
    // write data in pong buffer.
    m_canvasBuffer = m_canvasBuffer_pong;
    screenReset();
    CRGB tmp_bg  = CRGB(100,0,0);
    for (uint16_t x = 0; x < m_width; x++)
    {
        drawChar(x*5, 0, s[x], m_char_color, tmp_bg);
    }
    m_canvasBuffer = m_canvasBuffer_ping;

    float h_trg = 0.0;
    float h = m_height;

    while( h > h_trg)
    {
        // screenReset();
        animation(&h, &h_trg, 180); // 160
        for (int y = 0; y < m_height; y++)
        {
            if (y < int(h))
            {
                // uint16_t new_y = y + 1 >= m_height ? m_height - 1 : y + 1;
                // for (int x = 0; x < m_width; x++)
                // {
                //     m_canvasBuffer[XY(x,y)] = m_canvasBuffer[XY(x, new_y)];
                // } 
            }
            else{
                for (int x = 0; x < m_width; x++)
                {
                    // m_canvasBuffer[XY(x,y)] = m_canvasBuffer_pong[XY(x,m_height - y -1)]; // mirrow
                    m_canvasBuffer[XY(x,y)] = m_canvasBuffer_pong[XY(x, y -  int(h))];
                } 
            } 
        }
        render();
        // delay(30);
    }
   
}

// void slideCharUp(int8_t char_index, )
// void Display::fadeInAll() 
// {
//     // fadeLightBy(m_canvasBuffer, m_width*m_height, 10);
//     nscale8(m_canvasBuffer, m_width*m_height, 10);
// }


// }