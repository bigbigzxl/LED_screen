// This is the cmd vector.

#ifndef CMD_H
#define CMD_H
#include <vector>
#include <string>
#include <map>

#define CMD_NUM 20

std::vector<std::string> boot;
std::map<std::string, std::vector<std::string> > cmd;

boot.append(" HIBIK*");
boot.append(" HIBIK+");

cmd["INPT"] = {"AES", "COS", "USB", "I2S", "BLT"};
cmd["VOL"]  = {"000"}; // 0~200 uint8
cmd["PHAS"] = {"0", "180"};
cmd["PLL"]  = {"FAST", "ACCU"}; 
cmd["SYNC"] = {"INT", "EXT"};
cmd["OUPT"] = {"HPA", "PRE"};
cmd["AMPL"] = {"LO", "MI", "HI"};
cmd["TNT"]  = {"OFF", "ON"};
cmd["DSPL"] = {"CLR", "R00", "G00", "B00"};
cmd["ADIM"] = {"5S", "10S", "OFF"}; 
cmd["FW"]   = {"S001"};
cmd["SN"]   = {"S0xxx"};
cmd["DAT"]  = {"2209"};

// Standard cmd.
static const unsigned char boot[] = {
" ", "H", "I", "B", "I", "K", "I", "*",
" ", "H", "I", "B", "I", "K", "I", "+",
};


static const unsigned char cmd_keys[CMD_NUM][4] = {
{"I", "N", "P", "T"},
{"V", "O", "L",  ""},
{"P", "H", "A", "S"},
{"P", "L", "L",  ""},
{"S", "Y", "N", "C"},
{"O", "U", "P", "T"},
{"A", "M", "P", "L"},
{"T", "N", "T",  ""},
{"D", "S", "P", "L"},
{"A", "D", "I", "M"},
{"F", "W",  "",  ""},
{"S", "N",  "",  ""},
{"D", "A", "T",  ""}
};

// static const unsigned char cmd_values[CMD_NUM][4] = {
// {
// // {"A", "E", "S", ""},
// // {"C", "O", "S", ""},
// // {"U", "S", "B", ""},
// // {"I", "2", "S", ""},
// // {"B", "L", "T", ""}
// }
// };

// static const unsigned chat cmd_code[CMD_NUM][4] = {

// };

#endif // FONT5X7_H
