// -----
// Menu.h - Library for dealing with special logic.
// This class is implemented for use with the
// Arduino environment. Copyright (c) by Matthias Hertel,
// -----
// 05.04.2023 created by Xialong Zhang.
// -----

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <string.h>
#include "cmd/cmd.h"
#include "display/display.h"
// ----- Callback function types -----

extern "C" {
typedef void (*callbackFunction)(void);
typedef void (*parameterizedCallbackFunction)(void *);
}


class Menu
{
public:
    enum ButtonType : int
    {
        IDLE = 0,
        CLICK = 1,
        D_CLICK = 2, // double click
        L_CLICK = 3, // long click
        UNKNOW = 99
    };

    // define FiniteStateMachine
    enum StateMachine : int
    {
        POWEROFF = 0,
        POWERON,
        HOME,
        MENU,
        CMD,
        SET,
        EXE,
        DIY,
        GAME
    };

    struct color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    }
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
        {}
        uint32_t read_info_from_FPGA(void);
        bool write_info_to_FPGA(uint32_t infos);
        bool get_fpga_ready(void);

        uint32_t info_from_FPGA = 0;
        uint32_t info_to_FPGA = 0;
        bool mute = 0;
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
        unsigned char cur_test[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
        uint8_t cur_cmd_pos[13] ={0,0,0,0,0,0,0,0,0,0,0,0,0};

        unsigned long LOADING_TIME = 5000; // 5s
        unsigned long TIME_OUT     = 15000;    // 15s
    };
    
    static Menu* getInstance();

    // default constructor
    Menu() = default;

    void tick(void);

    void set_button_state(ButtonType t)
    {
        _buttonState = t;

        if (_cur_state == HOME)
        {
            _mute = !_mute;
        }
    }

    ButtonType get_button_state(ButtonType t)
    {
        return _buttonState;
    }

    StateMachine get_cur_state(void)
    {
        return _cur_state;
    }

    bool get_power_state()
    {
        return _power_state;
    }

    void set_power_state(bool pw)
    {
        _power_state = pw;
    }

    void update_delta(int d)
    {
        _delta = _delta == 0 ? d :  _delta + d;
    }

    void reset_delta(void)
    {
        _delta =  0;
    }
    /**
     *  @brief Advance to a new state and
     *  save the last one to come back
     *  in cas of bouncing detection.
     */
    void _newState(StateMachine nextState, unsigned long now)
    {
        _lastState = _cur_state;
        _cur_state = nextState;
        _startTime = now;
    }

    void _fresh_starttime( unsigned long now)
    {
        _startTime = now;
    }

    void FSM_tick(void); // only change status; no ops;
    void FSM_executor(void);

    void callback_boot(void);

    void callback_home(void);
    void callback_mute(void);
    void callback_volume(int8_t delta);

    void callback_spin_menu(void);
    void callback_spin_cmd(int8_t delta);
    void callback_spin_set(int8_t delta);


private:
    /**
     *  Advance to a new state and save the last one to come back
     *  in cas of bouncing detection.
     */
    void _newState(StateMachine nextState);

    StateMachine _cur_state = POWEROFF;
    StateMachine _lastState = POWEROFF; // used for timeout detect.
    unsigned long _startTime   = 0;      // start of current input change to checking timeout;

    volatile ButtonType _buttonState = IDLE;
    volatile  int32_t _delta = 0;

    volatile  bool _power_state = 0;         // 0 for power off; 1 for power on.
    volatile  bool _menu_selected_state = 0; // 0 for release state; 1 for selected state;

    menu_ctrl* menu_handle = nullptr;

    // callbackFunction _callback_boot      = NULL;
    // callbackFunction _callback_home      = NULL;
    // callbackFunction _callback_spin_menu = NULL;
    // callbackFunction _callback_spin_cmd  = NULL;

};
#endif