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
        EXE,
        DIY,
        GAME
    };

    // default constructor
    Menu() = default;

    void tick(void);

    void set_button_state(ButtonType t)
    {
        _buttonState = t;
    }

    ButtonType get_button_state(ButtonType t)
    {
        return _buttonState;
    }

    StateMachine get_cur_state(void)
    {
        return _state;
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
    /**
     *  @brief Advance to a new state and
     *  save the last one to come back
     *  in cas of bouncing detection.
     */
    void _newState(StateMachine nextState, unsigned long now)
    {
        _lastState = _state;
        _state = nextState;
        _startTime = now;
    }

    void attachBoot(callbackFunction newFunction)
    {
        _callback_boot = newFunction;
    }
    void attachHome(callbackFunction newFunction)
    {
        _callback_home = newFunction;
    }
    void attachSpinMenu(callbackFunction newFunction)
    {
        _callback_spin_menu = newFunction;
    }
    void attachSpinCmd(callbackFunction newFunction)
    {
        _callback_spin_cmd = newFunction;
    }

private:
    /**
     *  Advance to a new state and save the last one to come back
     *  in cas of bouncing detection.
     */
    void _newState(StateMachine nextState);

    StateMachine _state = POWEROFF;
    StateMachine _lastState = POWEROFF; // used for timeout detect.

    volatile ButtonType _buttonState = IDLE;
    volatile  int32_t _delta = 0;

    volatile  bool _power_state = 0;         // 0 for power off; 1 for power on.
    volatile  bool _menu_selected_state = 0; // 0 for release state; 1 for selected state;


    bool mute = false;
    unsigned char cur_vol = 50;

    unsigned long _startTime   = 0;      // start of current input change to checking timeout;
    unsigned long LOADING_TIME = 5000; // 5s
    unsigned long TIME_OUT     = 60000;    // 60s

    callbackFunction _callback_boot      = NULL;
    callbackFunction _callback_home      = NULL;
    callbackFunction _callback_spin_menu = NULL;
    callbackFunction _callback_spin_cmd  = NULL;

};
#endif