#include "menuCtrl.h"

void menu_ctrl::show_volume(void);
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
