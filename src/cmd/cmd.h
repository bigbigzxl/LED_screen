// This is the cmd vector.

#ifndef CMD_H
#define CMD_H
#include <Arduino.h>
#include <string.h>

// Standard cmd.
// const char* boot_str[];
// const char* menu[];
// const int32_t cmd_pos[]; 
// const char* cmd_items[];

extern const char* g_MENU[];
extern const int32_t g_CMD_POS[13];
extern const char* g_CMD_SET[];

extern int16_t g_cur_cmd_index;
extern int32_t g_all_cmd_num;
extern unsigned char g_cur_text[9];
extern uint8_t g_cur_cmd_pos[]; 
extern bool g_mute;
extern uint8_t g_cur_vol;
extern uint8_t g_char_r;
extern uint8_t g_char_g;
extern uint8_t g_char_b;
void set_voice_volume(unsigned char vol);
#endif // FONT5X7_H


// g_cur_cmd_pos // Use this for WS2812//