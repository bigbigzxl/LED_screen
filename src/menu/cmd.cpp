#include "cmd.h"

// Standard cmd.
// const char* boot_str[] = {
//                 " HIBIK*",
//                 " HIBIK+"
// };

const char* menu[] = {
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

const int32_t cmd_pos[] = {
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
const char* cmd_items[] = {
    "000", // 0~200 uint8
    "CLR", "R00", "G00", "B00",
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

float Voltage = 0.0;
int16_t cur_cmd_index = 0;
int32_t all_cmd_num = sizeof(cmd_pos) / sizeof(cmd_pos[0]);

unsigned char cur_text[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char* cur_cmd_values[] = {
    (char*)"000", // 0~200 uint8
    (char*)"CLR", 
    (char*)"AES",
    (char*)"  0",
    (char*)"FAST", 
    
    (char*)"INT", 
    (char*)"HPA", 
    (char*)"LO", 
    (char*)"OFF",
    (char*)" 5S", 
    
    (char*)"S001",
    (char*)"S0xxx",
    (char*)"2209"
}; 

void update_cmd_str(void)
{   
    // Serial.printf("strlen start.");
    int16_t menu_len = strlen(menu[cur_cmd_index]);
    int16_t cmd_len = strlen(cur_cmd_values[cur_cmd_index]);
    // Serial.printf("strlen done.");
    if (menu_len + cmd_len != 8)
    {
        Serial.printf("cmd length error!!!%s, %s, %d", menu_len, cmd_len, cmd_len);
        return;
    }
    
    
    // cur_text[0] += 1; 

    // char tmp[9];
    for (int i = 0; i < menu_len; i++)
    {
        cur_text[i] = *(menu[cur_cmd_index] + i);
    }

    for (int i = 0; i < cmd_len; i++)
    {
        cur_text[menu_len + i] = *(cur_cmd_values[cur_cmd_index] + i);
    }
    // tmp[8] = 0;

    // cur_text = tmp;
    // Serial.printf("%s\n", cur_text);

    // strcpy(cur_text, menu[cur_cmd_index]);
    // strcat(cur_text, cur_cmd_values[cur_cmd_index]);
}

void show_8char(unsigned char * s)
{
    for (int i = 0; i < 8; i++)
    {
        *(cur_text + i) = *(s + i);
    }
}

void show_voice_volume(unsigned char vol)
{   
    unsigned char unit    = 48 + vol / 1 % 10;
    unsigned char ten     = 48 + vol / 10 % 10;
    unsigned char hundred = 48 + vol / 100 % 10;
   
    unsigned char tmp[9] = {'V','O','L',' ',' ',' ',' ',' ', 0};

    if (hundred > 48) {tmp[5] = hundred;}

    if (ten > 48) {tmp[6] = ten;}
    else if (ten == 48 && hundred > 48) {tmp[6] = ten;}

    if (unit >= 48) {tmp[7] = unit;}

    // memcpy(cur_text, tmp, 8);
    show_8char(tmp);
}

void set_voice_volume(unsigned char vol){
    // just send vol code to FPGA.
}
