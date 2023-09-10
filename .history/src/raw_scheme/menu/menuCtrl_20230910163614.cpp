#include "menuCtrl.h"

void menu_ctrl::show_volume(void);

void menu_ctrl::show_booting(void)
{
    Display* screen = Display::getInstance();
    screen->drawstring((unsigned char *)" HIBIKI ");
    int8_t style = 0;
    if (style == 0)
    {
        uint16_t pix_pos[4] = {273, 274, 279, 278};

        uint32_t i = 0;
        unsigned long start = millis(); // current (relative) time in msecs.
        unsigned long end = start;
        CRGB* 
        while(end - start < 1000)
        {   
            screen->fadeOutChar(7, 150);
            i++;
            i %= 4;
            screen->get_buffer()[pix_pos[i]] = CRGB::Red;
            screen->render();
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
        while(end - start < 1000)
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


void menu_ctrl::show_home(void)
{
    uint32_t infos = get_info_from_FPGA();
    if (infos != info_from_FPGA)
    { // update
        info_from_FPGA = infos;
    }

    // parser infos from FPGA;

    // gen mapping string;

    // show
    Display* screen = Display::getInstance();
    screen->setCharColor(100,100,100);
    screen->drawstring((unsigned char *)"AES P441");
    screen->render();
    _fresh_starttime();
}
