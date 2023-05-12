#include "menuFSM.h"
#include "menu/cmd.h"

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
      _newState(HOME, now);
    }
    // else if (_lastState == POWERON)
    // {
    //   if (waitTime > LOADING_TIME)
    //   {
    //     _callback_home();
    //     _newState(HOME, now);
    //   }
    // }

    break;
  }

  case HOME:
  {
    // if (_lastState != HOME) {Serial.println("HOME");}
    
    // get info from FPGA;
    // get_info_from_FPGA();

    //show info
    

    //TODO: add  home items, like setting, wifi, etc. 

    // if (_lastState == POWERON || _lastState == HOME)
    // {
    //   break;
    // }

   if (_lastState == HOME)
    {
      if (waitTime > 3000 )
      {
        // Serial.println("power off, state epoll.");
        _callback_home();
        _startTime = now;
      }
    }

    // mute
    if (_buttonState == CLICK)
    {
      if (mute)
      {
        set_voice_volume(cur_vol);
        mute = false;
      }
      else
      {
        set_voice_volume(0);
      }

      _buttonState = IDLE;
    }

    if (_delta)
    {
      // Vol: 0~200
      cur_vol += _delta;
      cur_vol = cur_vol < 0 ? 0 : cur_vol;
      cur_vol = cur_vol > 200 ? 200 : cur_vol;
      _delta = 0;
      show_voice_volume(cur_vol);

      // Serial.printf("%s\n", cur_text);
      // Serial.printf("%d", cur_vol);
      // _callback_spin_menu();
      break;
    }


    if (_buttonState == D_CLICK)
    {
      _callback_spin_menu();
      _newState(MENU, now);

      _buttonState = IDLE;
      break;
    }
    // if (_buttonState == L_CLICK)
    // {
    //   _callback_home();
    //   _newState(HOME, now);
    //   _buttonState = IDLE;
    //   break;
    // }
    
    // state is not POWERON/HOME/DIY/GAME;
    // if (!(_lastState == DIY || _lastState == GAME) && waitTime >= TIME_OUT)
    // {
    //   _callback_home();
    //   _newState(HOME, now);
    //   break;
    // } 
    
    break;
  }

  case MENU:
  {
    // if (_lastState != MENU) {Serial.println("MENU");}
    
    if (_buttonState == D_CLICK)
    {
      _newState(CMD, now);
      _buttonState = IDLE;
      break;
    }

    if (_delta || _lastState != MENU)
    {
      cur_cmd_index += _delta;
      _delta = 0;
      _callback_spin_menu();
      break;
    }

    if (waitTime >= TIME_OUT || _buttonState == L_CLICK)
    {
      _callback_home();
      _newState(HOME, now);
      _buttonState = IDLE;
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