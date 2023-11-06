#include "menuCtrl.h"
#include "../user/cfg.h"


static menu_ctrl* g_menu_crtl = nullptr;

menu_ctrl::menu_ctrl()
: _all_menus_start(g_user_menus),

{
    _menu_items_num = sizeof(_all_menus_start) / sizeof(MenuItem);
    _cur_menu = _all_menus_start;
    _L1_menu_pos = new std::vector<int32_t>();
    menu_check();
    menu_init();

}

bool menu_ctrl::menu_init(void) const
{
    for (uint32_t pos = 0; pos < _menu_items_num; pos++)
    {
        if (_all_menus_start[i].level == MenuLevel::MENU_LIST_L1)
        {
            _L1_menu_pos->push_back(pos);
        }
    }

    if (_L1_menu_pos->size() == 0)
    {
        // show error
        return false;
    }

}

bool menu_ctrl::menu_check(void) const
{
    for (int i = 0; i < _menu_items_num; ++i)
    {
        
    }

    return true;
}

menu_ctrl* menu_ctrl::getInstance()
{
    if (g_menu_crtl == nullptr)
    {
        g_menu_crtl = new menu_ctrl();
        if (!g_menu_crtl->init())
        {
            delete g_menu_crtl;
            return nullptr;
        }
    }
    return g_menu_crtl;
}
void menu_ctrl::show_volume(void)
{
    unsigned char unit    = 48 + cur_vol / 1 % 10;
    unsigned char ten     = 48 + cur_vol / 10 % 10;
    unsigned char hundred = 48 + cur_vol / 100 % 10;

    unsigned char tmp[8] = {'V','O','L',' ',' ',' ',' ',' '};

    if (hundred > 48) {tmp[5] = hundred;}

    if (ten > 48)                       { tmp[6] = ten; }
    else if (ten == 48 && hundred > 48) { tmp[6] = ten; }

    if (unit >= 48) {tmp[7] = unit;}

    Display* screen = Display::getInstance();
    screen->drawstring(tmp);
    screen->render();
}

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
        // CRGB* buffer = screen->get_buffer();
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
            screen->fadeOutChar(7, 240);
            i++;
            i %= 4;
            screen->m_canvasBuffer[pix_pos[i]] = CRGB::Red;
            screen->render();
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
}

void menu_ctrl::spin_L1_menu(int32_t delta)
{
    if (_cur_menu->level != MenuLevel::MENU_LIST_L1)
    {
        return;
    }

    //TODO: animation code.
    while(delta--)
    {
        _cur_menu->parameterizedCallbackFunction(1); // forward one step;
        // time delay
    }
}

uint32_t menu_ctrl::get_info_from_FPGA(void)
{
    uint32_t info_r = 0;
    // get info

    // check info format.

    return info_r;
}

bool menu_ctrl::write_info_to_FPGA(uint32_t infos)
{
    return true;
}

bool menu_ctrl::get_fpga_ready(void)
{
    return true;
}

bool menu_ctrl::info_format_check(uint32_t info)
{
    return true;
}

char* menu_ctrl::parser_infos(uint32_t info)
{
    uint8_t info_2_idx = 0;
    return nullptr;
}
