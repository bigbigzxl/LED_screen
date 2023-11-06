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
#include "../render/display.h"
#include "../ctrl/menuCtrl.h"

class MenuFsm
{
public:
    // define FiniteStateMachine
    enum StateMachine : int
    {
        POWEROFF = 0,
        POWERON,
        HOME,
        VOL,
        MUTE,
        MENU,
        CMD,
        SET,
        EXE,
        DIY,
        GAME
    };

    static MenuFsm* getInstance();
    void tick(void);
    menu_ctrl* get_ctrl_handle()
    {
        return _menu_handle;
    }
private:
    /**
     *  Advance to a new state and save the last one to come back
     *  in cas of bouncing detection.
     */
    // default constructor
    MenuFsm();
    void _newState(StateMachine nextState);
    void _FSM_tick(unsigned long now); // only change status; no ops;
    void _FSM_executor(unsigned long now);
    
    INLINE void _fresh_starttime( unsigned long now)
    {
        _startTime = now;
    }

    INLINE StateMachine _get_cur_state(void)
    {
        return _cur_state;
    }
    /**
     *  @brief Advance to a new state and
     *  save the last one to come back
     *  in cas of bouncing detection.
     */
    INLINE void _newState(StateMachine nextState, unsigned long now)
    {
        _lastState = _cur_state;
        _cur_state = nextState;
        _startTime = now;
    }

    StateMachine _cur_state;
    StateMachine _lastState; // used for timeout detect.
    unsigned long _startTime;      // start of current input change to checking timeout;
    menu_ctrl* _menu_handle;
};
#endif