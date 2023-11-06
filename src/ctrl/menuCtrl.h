#ifndef MENU_CTRL_H
#define MENU_CTRL_H
#include <vector>
#include "../render/display.h"

// ----- Callback function types -----
extern "C" {
typedef void (*callbackFunction)(void);
// typedef void (*parameterizedCallbackFunction)(void *);
typedef void (*parameterizedCallbackFunction)(int32_t param);
}

enum ButtonType : int
{
    IDLE = 0,
    CLICK = 1,
    D_CLICK = 2, // double click
    L_CLICK = 3, // long click
    UNKNOW = 99
};

struct color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

typedef enum
{
    MENU_LIST_L1 = 0,   // entry
    MENU_CMD,       // cmd
    // can set more levels.
    MENU_SET,  // set
} MenuLevel;

struct MenuItem
{
    MenuLevel level;
    char* name;
    int32_t pos_value; //MENU_LIST_L1: cur cmd pos; other: cur value; 
    bool (*parameterizedCallbackFunction) (int32_t param);
};

class menu_ctrl
{
public:
    void update_volume(int32_t v_delta)
    {
        cur_vol += v_delta;

        // clip
        cur_vol = cur_vol < 0 ? 0 : cur_vol;
        cur_vol = cur_vol > 200 ? 200 : cur_vol;
    }
    void set_voice_volume(int32_t v)
    {
        // clip
        cur_vol = v < 0 ? 0 : v;
        cur_vol = v > 200 ? 200 : v;
    }

    static menu_ctrl* getInstance(void);

    /*********I/O statue**********************/
    bool is_powerON()
    {
        // 1: on
        // 0: off
        return _power_state == 1 ? true : false;
    }

    bool is_powerOFF()
    {
        // 1: on
        // 0: off
        return _power_state == 0 ? true : false;
    }

    void set_powerON(void)
    {
        _power_state = 1;
    }

    void set_powerOFF(void)
    {
        _power_state = 0;
    }

    int32_t _get_delta(void)
    {
        return _delta;
    }

    void _update_delta(int32_t d)
    {
        _delta += d;
    }

    void _reset_delta(void)
    {
        _delta =  0;
    }

    void _set_button_state(ButtonType t)
    {
        _buttonState = t;
    }

    ButtonType _get_button_state(ButtonType t)
    {
        return _buttonState;
    }

    void _release_button(void)
    {
        _buttonState = ButtonType::IDLE;
    }
    
    bool _is_button_click()
    {
        return _buttonState == CLICK;
    }

    bool _is_button_doubleclick()
    {
        return _buttonState == D_CLICK;
    }

    bool _is_button_longclick()
    {
        return _buttonState == L_CLICK;
    }

    bool _is_home_timeout(int32_t wait_time)
    {
        return wait_time > HOME_TIMEOUT;
    }

    bool _is_loading_timeout(int32_t wait_time)
    {
        return wait_time > LOADING_TIMEOUT;
    }

    bool _is_volume_timeout(int32_t wait_time)
    {
        return wait_time > VOL_SHOW_TIMEOUT;
    }

    uint32_t get_info_from_FPGA(void);
    bool write_info_to_FPGA(uint32_t infos);
    bool get_fpga_ready(void);
    bool info_format_check(uint32_t info);

    char* parser_infos(uint32_t info);

    bool menu_init(void) const;
    bool menu_check(const MenuItem* menus) const;

    void show_volume(void);
    void show_home(void);
    void show_booting(void);
    void init_menu_list(void);

    void spin_L1_menu(int32_t delta);
    uint32_t info_from_FPGA = 0;
    uint32_t info_to_FPGA = 0;
    bool mute = false;
    bool vol_changed = false;
    uint8_t cur_vol = 50;
    color rgb = {99,99,99}; 
    // char* menu_name[] = {
    //         "VOL  ",
    //         "DSPL ",
    //         "INPT ", 
    //         "PHAS ",
    //         "PLL ",

    //         "SYNC ",
    //         "OUPT ",
    //         "AMPL  ",
    //         "TNT  ",
    //         "ADIM ",

    //         "FW  ",
    //         "SN ",
    //         "DAT " 
    
    // };
    // int32_t cmd_pos[] = {
    //         0,
    //         1,
    //         5,
    //         10,
    //         12,

    //         14,
    //         16,
    //         18,
    //         21,
    //         23,

    //         26,
    //         27,
    //         28
    // };
    // char* cmd_set[] = {
    //     "000", // 0~200 uint8
    //     "CLR", "R00", "G00", "B00",// 0 ~255

    //     "AES", "COS", "USB", "I2S", "BLT",
    //     "  0", "180",
    //     "FAST", "ACCU",
        
    //     "INT", "EXT",
    //     "HPA", "PRE",
    //     "LO", "MI", "HI",
    //     "OFF", " ON",
    //     " 5S", "10S", "OFF",

    //     "S001",
    //     "S0xxx",
    //     "2209"
    // };
    
    
    
    int16_t cur_cmd_index = 0;
    int16_t all_cmd_num = 0; // sizeof(all_menus) / sizeof(MenuItem);;
    unsigned char cur_text[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};


    MenuItem *_all_menus_start = nullptr;
    MenuItem *_cur_menu = nullptr;
    uint32_t _menu_items_num = 0;
    std::vector<int32_t>* _L1_menu_pos;

    unsigned long LOADING_TIMEOUT   = 5000; // 5s
    unsigned long VOL_SHOW_TIMEOUT  = 1000; // 1s
    unsigned long HOME_TIMEOUT      = 15000;    // 15s

    volatile  bool _power_state = 0;         // 0 for power off; 1 for power on.
    volatile  bool _menu_selected_state = 0; // 0 for release state; 1 for selected state;
    volatile  ButtonType _buttonState = IDLE;
    volatile  int32_t _delta = 0;
    unsigned long _startTime;      // start of current input change to checking timeout;
};

#endif