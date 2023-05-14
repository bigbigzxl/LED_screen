#include "menuFSM.h"


static Menu* g_FSM = nullptr;

Menu* Menu::getInstance()
{
    if (g_FSM == nullptr)
    {
        g_FSM = new Menu();
    }
    return g_FSM;
}

void Menu::callback_boot(void)
{
  Serial.println("call_back: boot");
  Display* screen = Display::getInstance();
  screen->showLogo(0);
}

void Menu::callback_mute(bool isMute)
{
  if (isMute)
  {
    Serial.println("call_back: mute");
    Display* screen = Display::getInstance();
    screen->drawstring((unsigned char *)"  Mute  ");
    screen->render();
  }
  else
  {
    callback_home();
  }

}

void Menu::callback_volume(uint8_t vol)
{
    vol = vol < 0 ? 0 : vol;
    vol = vol > 200 ? 200 : vol;
      
    unsigned char unit    = 48 + vol / 1 % 10;
    unsigned char ten     = 48 + vol / 10 % 10;
    unsigned char hundred = 48 + vol / 100 % 10;
   
    unsigned char tmp[8] = {'V','O','L',' ',' ',' ',' ',' '};

    if (hundred > 48) {tmp[5] = hundred;}

    if (ten > 48) {tmp[6] = ten;}
    else if (ten == 48 && hundred > 48) {tmp[6] = ten;}

    if (unit >= 48) {tmp[7] = unit;}

    Display* screen = Display::getInstance();
    screen->drawstring(tmp);
    screen->render();
}



void Menu::callback_home()
{
    Serial.println("call_back: home");
    Display* screen = Display::getInstance();
    screen->drawstring((unsigned char *)"AES P441");
    screen->render();
}


void Menu::callback_spin_menu()
{        
    Serial.println("call_back: spin_menu");
    if (cur_cmd_index < 0)
    {
        cur_cmd_index = (cur_cmd_index + all_cmd_num) % all_cmd_num;
    }
    else
    {
        cur_cmd_index %= all_cmd_num;
    }
    
    update_cmd_str();

    Display* screen = Display::getInstance();
    screen->drawstring(cur_text);
    screen->render();
}

void Menu::callback_spin_cmd()
{
    // director->runWithScene(dot2d::Matrix::create()); // power on LOGO render.
    Serial.println("call_back: spin_cmd");
}



void Menu::tick()
{
  unsigned long now = millis(); // current (relative) time in msecs.
  unsigned long waitTime = (now - _startTime);

  if (!_power_state && _state != POWEROFF) // poweroff 
  {
    _newState(POWEROFF, now);
  }

  // Implementation of the state machine.
  switch (_state)
  {
  case POWEROFF: // IDLE statue
  {
    // if (_lastState != POWEROFF) {Serial.println("POWEROFF");}

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

      if (_buttonState == L_CLICK)
      {
        _newState(POWERON, now);
        _power_state = true;
        _buttonState = IDLE;
         Serial.println("enter POWERON state.");
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
    // if (_lastState != POWERON) {Serial.println("POWERON");}

    if (_lastState == POWEROFF)
    {
      // boot: attach boot function from main;
      Serial.println("start call boot process.");
      callback_boot();
      _newState(HOME, now);
       Serial.println("start enter HOME state.");
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
    if (waitTime > 3000 && !_delta && !m_mute)
    {
      // Serial.println("power off, state epoll.");
      callback_home();
    }
  //  if (_lastState == HOME)
  //   {

  //   }

    // mute
    if (_buttonState == CLICK)
    {
      if (m_mute)
      {
        set_voice_volume(m_cur_vol);
        m_mute = false;
      }
      else
      {
        m_mute = true;
        set_voice_volume(0);
      }

      callback_mute(m_mute);

      _buttonState = IDLE;
      _startTime = now;
      break;
    }

    if (_delta && !m_mute)
    {
      // Vol: 0~200
      m_cur_vol += _delta;
      _delta = 0;
      callback_volume(m_cur_vol);
      _startTime = now;
      break;
    }

    if (_buttonState == D_CLICK && !m_mute)
    {
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
      callback_spin_menu();
      break;
    }

    if (waitTime >= TIME_OUT || _buttonState == L_CLICK)
    {
      callback_home();
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
      callback_spin_cmd();
      break;
    }
    break;
  }
  
  default:
    break;
  }

}
