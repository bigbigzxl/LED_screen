#include "menu.h"

void Menu::tick()
{
  unsigned long now = millis(); // current (relative) time in msecs.
  unsigned long waitTime = (now - _startTime);

  if (!_power_state) // poweroff 
  {
    _newState(POWEROFF, now);
  }

  // Implementation of the state machine.
  switch (_state)
  {
  case POWEROFF: // IDLE statue
  {
    Serial.println("power off");

    if (_power_state) // poweron 
    {
      _newState(POWERON, now);
    }

    if (_lastState == POWEROFF )
    {
      Serial.println("power off, state epoll.");
      vTaskDelay(300 / portTICK_PERIOD_MS);
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
    Serial.println("power on");

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
    Serial.println("home");

    if (_lastState == POWERON || _lastState == HOME)
    {
      break;
    }

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
    Serial.println("menu");

    if (_lastState == CMD && _buttonState == CLICK)
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
      _callback_spin_menu();
      break;
    }

    break;
  }

  case CMD:
  {
    Serial.println("cmd");

    if (_lastState == MENU && _buttonState == CLICK)
    {
      _newState(CMD, now);
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
      _callback_spin_cmd();
      break;
    }
    break;
  }
  
  default:
    break;
  }

}
