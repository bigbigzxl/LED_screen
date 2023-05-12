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

extern float Voltage;
extern int16_t cur_cmd_index;
extern int32_t all_cmd_num;
extern unsigned char cur_text[9];
extern char* cur_cmd_values[]; 

void update_cmd_str(void);

void show_8char(unsigned char * s);

void show_voice_volume(unsigned char vol);

void set_voice_volume(unsigned char vol);
#endif // FONT5X7_H
