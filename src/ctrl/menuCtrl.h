#ifndef MENU_CTRL_H
#define MENU_CTRL_H

#include "../render/display.h"

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
    bool (*callback) (int32_t param);
};

class menu_ctrl
{
public:
    static menu_ctrl* getInstance()
    {
        if (g_menu_crtl == nullptr)
        {
            g_menu_crtl = new menu_ctrl();
        }
        return g_menu_crtl;
    }

    void update_volume(int32_t v_delta)
    {
        cur_vol += v_delta;

        // clip
        cur_vol = cur_vol < 0 ? 0 : cur_vol;
        cur_vol = cur_vol > 200 ? 200 : cur_vol;
    }

    uint32_t read_info_from_FPGA(void);
    bool write_info_to_FPGA(uint32_t infos);
    bool get_fpga_ready(void);

    char* parser_infos(uint32_t info);

    void show_volume(void);
    void show_home(void);
    void show_booting(void);
    void init_menu_list();
    uint32_t info_from_FPGA = 0;
    uint32_t info_to_FPGA = 0;
    bool mute = false;
    bool vol_changed = false;
    uint8_t cur_vol = 50;
    color rgb = {99,99,99}; 
    char* menu_name[] = {
            "VOL  ",
            "DSPL ",
            "INPT ", 
            "PHAS ",
            "PLL ",

            "SYNC ",
            "OUPT ",
            "AMPL  ",
            "TNT  ",
            "ADIM ",

            "FW  ",
            "SN ",
            "DAT " 
    
    };
    int32_t cmd_pos[] = {
            0,
            1,
            5,
            10,
            12,

            14,
            16,
            18,
            21,
            23,

            26,
            27,
            28
    };
    char* cmd_set[] = {
        "000", // 0~200 uint8
        "CLR", "R00", "G00", "B00",// 0 ~255

        "AES", "COS", "USB", "I2S", "BLT",
        "  0", "180",
        "FAST", "ACCU",
        
        "INT", "EXT",
        "HPA", "PRE",
        "LO", "MI", "HI",
        "OFF", " ON",
        " 5S", "10S", "OFF",

        "S001",
        "S0xxx",
        "2209"
    };
    int16_t cur_cmd_index = 0;
    int16_t all_cmd_num = sizeof(cmd_pos) / sizeof(cmd_pos[0]);;
    unsigned char cur_text[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    uint8_t cur_cmd_pos[13] ={0,0,0,0,0,0,0,0,0,0,0,0,0};

    MenuItem *all_menus = nullptr;
    unsigned long LOADING_TIME  = 5000; // 5s
    unsigned long VOL_SHOW_TIME = 1000; // 1s
    unsigned long TIME_OUT      = 15000;    // 15s
};

#endif