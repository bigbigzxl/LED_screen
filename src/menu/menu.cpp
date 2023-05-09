#include "menu.h"

void Menu::tick()
{
  unsigned long now = millis(); // current (relative) time in msecs.
  unsigned long waitTime = (now - _startTime);

  if (!_power_state) // poweroff 
  {
    _newState(POWEROFF, now);
    return;
  }

  // Implementation of the state machine.
  switch (_state)
  {
  case POWEROFF: // IDLE statue
  {
    if (_lastState != POWEROFF) {Serial.println("POWEROFF");}

    if (_power_state) // poweron 
    {
      _newState(POWERON, now);
    }

    if (_lastState == POWEROFF)
    {
      if (waitTime > 3000 )
      {
        Serial.println("power off, state epoll.");
        _startTime = now;
        // vTaskDelay(3000 / portTICK_PERIOD_MS);
      }
    }
    else
    { /* shutdown. */
    
      // save configs

      // send power off IR

      // close the screen.

      _newState(POWEROFF, now);

    }
    break;
  }

  case POWERON:
  {
    if (_lastState != POWERON) {Serial.println("POWERON");}

    if (_lastState == POWEROFF)
    {
      // boot: attach boot function from main;
      _callback_boot();
      _newState(POWERON, now);
    }
    else if (_lastState == POWERON)
    {
      if (waitTime > LOADING_TIME)
      {
        _callback_home();
        _newState(HOME, now);
      }
    }

    break;
  }

  case HOME:
  {
    if (_lastState != HOME) {Serial.println("HOME");}

    _newState(MENU, now);
    break;


    //TODO: add  home items, like setting, wifi, etc. 

    // if (_lastState == POWERON || _lastState == HOME)
    // {
    //   break;
    // }

    if (_buttonState == L_CLICK)
    {
      _callback_home();
      _newState(HOME, now);
      _buttonState = IDLE;
      break;
    }
    
    // state is not POWERON/HOME/DIY/GAME;
    if (!(_lastState == DIY || _lastState == GAME) && waitTime >= TIME_OUT)
    {
      _callback_home();
      _newState(HOME, now);
      break;
    } 
    
    break;
  }

  case MENU:
  {
    if (_lastState != MENU) {Serial.println("MENU");}
    

    if (_buttonState == CLICK)
    {
      _newState(CMD, now);
      _buttonState = IDLE;
      break;
    }

    if (waitTime >= TIME_OUT || _buttonState == L_CLICK)
    {
      _newState(HOME, now);
      _buttonState = IDLE;
      break;
    }    

    if (_delta)
    {
      Serial.printf("%d", _delta);
      _callback_spin_menu();
      break;
    }

    break;
  }

  case CMD:
  {
   if (_lastState != CMD) {Serial.println("CMD");}

    if (_buttonState == CLICK)
    {
      _newState(MENU, now);
      _buttonState = IDLE;
      break;
    }

    if (_buttonState == L_CLICK || waitTime > TIME_OUT)
    {
       _newState(HOME, now);
      _buttonState = IDLE;
      break;
    }

    if (_delta)
    {
      Serial.printf("%d", _delta);
      _callback_spin_cmd();
      break;
    }
    break;
  }
  
  default:
    break;
  }

}
