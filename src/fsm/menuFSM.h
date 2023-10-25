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

    // default constructor
    MenuFsm() = default;

    void tick(void);

    void _set_button_state(ButtonType t)
    {
        _buttonState = t;

        if (_cur_state == HOME)
        {
            menu_handle->mute = !menu_handle->mute;
        }
    }

    ButtonType _get_button_state(ButtonType t)
    {
        return _buttonState;
    }

    StateMachine _get_cur_state(void)
    {
        return _cur_state;
    }

    INLINE bool _get_power_state()
    {
        return _power_state;
    }

    INLINE void _set_power_state(bool pw)
    {
        _power_state = pw;
    }

    INLINE void _update_delta(int d)
    {
        _delta += d;
    }

    INLINE void _reset_delta(void)
    {
        _delta =  0;
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

    INLINE void _fresh_starttime( unsigned long now)
    {
        _startTime = now;
    }

    INLINE void _release_button( unsigned long now)
    {
        _buttonState = ButtonType::IDLE;
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