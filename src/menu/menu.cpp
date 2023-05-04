#include "menu.h"

Menu::Menu()
{
}

void Menu::tick(bool entered)
{
  unsigned long now = millis(); // current (relative) time in msecs.
  unsigned long waitTime = (now - _startTime);

  // Implementation of the state machine.
  switch (_state)
  {
  case POWEROFF: // IDLE statue
  {
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
    if (!_power_state) // poweroff 
    {
      _newState(POWEROFF, now);
    }

    if (_lastState == POWEROFF)
    {
      // boot: attach boot function from main;
      callback_boot();
      _newState(POWERON, now);
    }
    else if (_lastState == POWERON)
    {
      if (waitTime > LOADING_TIME)
      {
        callback_home();
        _newState(HOME, now);
      }
    }

    break;
  }
  case HOME:
  {
    if (_lastState == POWERON || _lastState == HOME)
    {
      break;
    }

    if (_lastState == DIY || _lastState == GAME)
    {
      if (_buttonState == L_CLICK)
      {
        callback_home();
        _newState(HOME, now);
      }
      break;
    }
    // state is not POWERON/HOME/DIY/GAME;
    if (_buttonState == L_CLICK || waitTime > TIME_OUT)
    {
      callback_home();
      _newState(HOME, now);
    }
    break;
  }
  case MENU:
  {
    if (_lastState == DIY)
    {
      // TODO: add logic.
      break;
    }

    
    break;
  }
  break;
  }

default:
  break;
}
