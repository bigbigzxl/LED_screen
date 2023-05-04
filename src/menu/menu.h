// -----
// Menu.h - Library for dealing with special logic.
// This class is implemented for use with the
// Arduino environment. Copyright (c) by Matthias Hertel,
// -----
// 05.04.2023 created by Xialong Zhang.
// -----

#ifndef MENU_H
#define MENU_H

class Menu
{
public:
    // default constructor
    Menu();
    


private:
    // define FiniteStateMachine
    enum stateMachine_t : int {
        HOME = 0,
        MENU = 1,
        CMD  = 2,
        EXE  = 3,
        UNKNOW = 99
    };

    /**
     *  Advance to a new state and save the last one to come back
     *  in cas of bouncing detection.
     */
    void _newState(stateMachine_t nextState);

    stateMachine_t _state = HOME;
    stateMachine_t _lastState = HOME; // used for timeout detect.

    unsigned long _startTime; // start of current input change to checking timeout;
     
    
};
#endif