#include "cmd.h"

// Standard cmd.
// const char* boot_str[] = {
//                 " HIBIK*",
//                 " HIBIK+"
// };

const char* g_MENU[] = {
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

const int32_t g_CMD_POS[] = {
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
const char* g_CMD_SET[] = {
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


int16_t g_cur_cmd_index = 0;
int32_t g_all_cmd_num = sizeof(g_CMD_POS) / sizeof(g_CMD_POS[0]);
bool g_mute = false;
uint8_t g_cur_vol = 50;

uint8_t g_char_r = 99;
uint8_t g_char_g = 99;
uint8_t g_char_b = 99;

unsigned char g_cur_text[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
uint8_t g_cur_cmd_pos[] = {
0,0,0,0,0,
0,0,0,0,0,
0,0,0
}; 

void set_voice_volume(unsigned char vol){
    // just send vol code to FPGA.
}
