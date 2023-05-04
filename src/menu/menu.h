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
        GAME,
        UNKNOW
    };

    // default constructor
    Menu() = default;

    void tick(bool entered);
    void set_button_state(ButtonType t)
    {
        _buttonState = t;
    }

    ButtonType get_button_state(ButtonType t)
    {
        return _buttonState;
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

private:
    /**
     *  Advance to a new state and save the last one to come back
     *  in cas of bouncing detection.
     */
    void _newState(StateMachine nextState);

    StateMachine _state = POWEROFF;
    StateMachine _lastState = POWEROFF; // used for timeout detect.

    ButtonType _buttonState = IDLE;
    int32_t _delta = 0;

    bool _power_state = 0;         // 0 for power off; 1 for power on.
    bool _menu_selected_state = 0; // 0 for release state; 1 for selected state;

    unsigned long _startTime = 0;      // start of current input change to checking timeout;
    unsigned long LOADING_TIME = 5000; // 5s
    unsigned long TIME_OUT = 15000;    // 15s
};
#endif