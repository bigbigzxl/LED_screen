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

void Menu::callback_mute()
{
  g_mute = !g_mute;

  if (g_mute)
  {
    set_voice_volume(0);
    Display* screen = Display::getInstance();
    screen->drawstring((unsigned char *)"  Mute  ");
    screen->render();
    // Serial.println("call_back: mute");
  }
  else
  {
    set_voice_volume(g_cur_vol);
    callback_home();
  }
}

void Menu::callback_volume(int8_t delta)
{
    int16_t cur_vol = g_cur_vol + delta;
    cur_vol = cur_vol < 0 ? 0 : cur_vol;
    cur_vol = cur_vol > 200 ? 200 : cur_vol;
    g_cur_vol = (uint8_t)cur_vol;
      
    unsigned char unit    = 48 + g_cur_vol / 1 % 10;
    unsigned char ten     = 48 + g_cur_vol / 10 % 10;
    unsigned char hundred = 48 + g_cur_vol / 100 % 10;
   
    unsigned char tmp[8] = {'V','O','L',' ',' ',' ',' ',' '};

    if (hundred > 48) {tmp[5] = hundred;}

    if (ten > 48)
    {
      tmp[6] = ten;
    }
    else if (ten == 48 && hundred > 48)
    {
      tmp[6] = ten;
    }

    if (unit >= 48) {tmp[7] = unit;}

    Display* screen = Display::getInstance();
    screen->drawstring(tmp);
    screen->render();
}



void Menu::callback_home()
{
    Serial.println("call_back: home");
    Display* screen = Display::getInstance();
    screen->setCharColor(100,100,100);
    screen->drawstring((unsigned char *)"AES P441");
    screen->render();
}


void Menu::callback_spin_menu()
{        
    while (g_cur_cmd_index < 0)
    {
        g_cur_cmd_index = (g_cur_cmd_index + g_all_cmd_num) % g_all_cmd_num;
    }
    
    g_cur_cmd_index %= g_all_cmd_num;
    
    
    int16_t menu_len = strlen(g_MENU[g_cur_cmd_index]);
    const char* cur_cmd = g_CMD_SET[g_CMD_POS[g_cur_cmd_index] + g_cur_cmd_pos[g_cur_cmd_index]];
    int16_t cmd_len = strlen(cur_cmd);
    // Serial.printf("strlen done.");
    if (menu_len + cmd_len != 8)
    {
        Serial.printf("cmd length error!!!%s, %s, %d", menu_len, cmd_len, cmd_len);
        return;
    }
    
    for (int i = 0; i < menu_len; i++)
    {
        g_cur_text[i] = *(g_MENU[g_cur_cmd_index] + i);
    }

    for (int i = 0; i < cmd_len; i++)
    {
        g_cur_text[menu_len + i] = *(cur_cmd + i);
    }

    if (g_cur_cmd_index == 0)
    {
      // show current vol
    }
    else if (g_cur_cmd_index == 1)
    {
      // show RGB

    }
    
    Display* screen = Display::getInstance();
    screen->setCharColor(0,100,0);
    // screen->drawstring(g_cur_text);
    screen->drawstring_slide_in(g_cur_text);
    
    screen->render();
    Serial.println("call_back: spin_menu");
}

void Menu::callback_spin_set(int8_t delta)
{

}


void Menu::callback_spin_cmd(int8_t delta)
{

  Display* screen = Display::getInstance();

  // TODO: spark background.
  if (g_cur_cmd_index == 0)
  {
    // adjust volume.
    callback_volume(delta);
    return;
  }

  int8_t cmd_candidate_num = g_CMD_POS[g_cur_cmd_index+1] - g_CMD_POS[g_cur_cmd_index];
  if ( cmd_candidate_num == 1) {return;} // can not modify.

  delta += g_cur_cmd_pos[g_cur_cmd_index];
  while(delta < 0)
  {
    delta += cmd_candidate_num;
  }
  delta %= cmd_candidate_num;

  const char* selected_cmd = g_CMD_SET[g_CMD_POS[g_cur_cmd_index] + delta];
  Serial.printf("%s\n", selected_cmd);

  uint8_t key_len = strlen(g_MENU[g_cur_cmd_index]);

  if (g_cur_cmd_index == 1 && delta > 0)
  {
    if (delta == 1)
    {
      uint8_t r = 100 * g_char_r / 255.0;
      g_cur_text[5] = 'R';
      g_cur_text[6] = '0' + r / 10;
      g_cur_text[7] = '0' + r % 10;
    }
    else if (delta == 2)
    {
      uint8_t g = 100 * g_char_g / 255.0;
      g_cur_text[5] = 'G';
      g_cur_text[6] = '0' + g / 10;
      g_cur_text[7] = '0' + g % 10;
    }
    else if (delta == 3)
    {
      uint8_t b = 100 * g_char_b / 255.0;
      g_cur_text[5] = 'B';
      g_cur_text[6] = '0' + b / 10;
      g_cur_text[7] = '0' + b % 10;
    }
    else
    {
      Serial.println("call_back: spin_cmd; ERROR: DISP: RGB index err..");
      return;
    }
  }
  else
  {
    for (uint8_t i = 0; i < 8 - key_len; i++)
    {
        g_cur_text[i + key_len] = *(selected_cmd + i);
    }
    // update g_cur_cmd_pos;
    g_cur_cmd_pos[g_cur_cmd_index] = delta;
  }
  screen->setCharColor(100,0,0);
  screen->drawstring(g_cur_text);
  screen->render();
  // int count = 100;
  // while(count--)
  // {
  //     if (count > 75)
  //     {
  //         // screen.fadeOutChar(5, 1);
  //         screen.fadeOutAll(1);
  //     }
  //     else
  //     {
  //         // screen.fadeOutChar(5, 5);
  //         screen.fadeOutAll(10);
  //     }
  //     //  screen.fadeOutChar(5, 10);
  //     screen.render();
  //     //  delay(10);
  // }
  Serial.println("call_back: spin_cmd");
}

uint32_t MENU::read_info_from_FPGA(void)
{
  return 0;
};
bool MENU::write_info_to_FPGA(uint32_t infos)
{
  return 1;
};

void MENU::FSM_tick(unsigned long now)
{
  unsigned long waitTime = (now - _startTime);

  switch (_cur_state)
  {
    // hight prior;
    // power off trigered or init power-off state;
    if (!_power_state) 
    {
      _newState(POWEROFF, now);
    }

    case POWER_OFF:
    {
      if (_power_state)
      {
        _newState(POWERON, now);
      }
      break;
    }

    case POWERON:
    {
      // do something before enter home page.
      if (waitTime > LOADING_TIME)
      {
        _newState(HOME, now);
      }
      break;
    }

    case HOME:
    {
      if (_buttonState == D_CLICK && !_mute)
      {
        _newState(MENU, now);
      }
      break;
    }

    case MENU:
    {
      if (_buttonState == D_CLICK)
      {
        _newState(CMD, now);
      }
      else if (_buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }

    case CMD:
    {
      if (_buttonState == D_CLICK)
      {
        _newState(SET, now);
      }
      else if (_buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }

    case SET:
    {
      if (_buttonState == D_CLICK)
      {
        _newState(CMD, now);
      }
      else if (_buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }
  }
}

void MENU::FSM_executor(unsigned long now)
{
  unsigned long waitTime = (now - _startTime);

  switch (_cur_state)
  {
    case POWER_OFF:
    {
      if (_lastState == POWERON)
      {
        // save context and clear the screen;
        // step 1: save context;
        // TODO: eq. save params to eeprom;

        // step 2: clear the screen.
        Display* screen = Display::getInstance();
        screen->screenReset();
      }
      break;
    }

    case POWERON:
    {
      // do something before enter home page.
      if (waitTime < LOADING_TIME)
      {
        callback_boot();
      }
      break;
    }

    case HOME:
    {
      if (waitTime > TIME_OUT)
      {
        Display* screen = Display::getInstance();
        screen->setCharColor(100,0,0);
        screen->drawstring((unsigned char *)"AES P441");
        screen->render();
      }
      uint32_t infos = get_info_from_FPGA();
      if (infos)
      {
        callback_home();
      }
      break;
    }

    case MENU:
    {
      if (_buttonState == D_CLICK)
      {
        _newState(CMD, now);
      }
      else if (_buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }

    case CMD:
    {
      if (_buttonState == D_CLICK)
      {
        _newState(SET, now);
      }
      else if (_buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }

    case SET:
    {
      if (_buttonState == D_CLICK)
      {
        _newState(CMD, now);
      }
      else if (_buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }
  }
}


void Menu::tick()
{
  unsigned long now = millis(); // current (relative) time in msecs.
  unsigned long waitTime = (now - _startTime);

  if (!_power_state && _cur_state != POWEROFF) // poweroff 
  {
    _newState(POWEROFF, now);
  }

  // Implementation of the state machine.
  switch (_cur_state)
  {
    case POWEROFF: // IDLE statue
    {
      // if (_lastState != POWEROFF) {Serial.println("POWEROFF");}

      if (_power_state) // poweron 
      {
        _newState(POWERON, now);
        break;
      }

      if (_lastState == POWEROFF)
      {
        if (waitTime > 5000 )
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

        if (_buttonState == CLICK)
        {
          _buttonState = IDLE;
          Serial.println("click.");
          Display* screen = Display::getInstance();
          screen->setCharColor(100,100,100);

          screen->drawstring_slide_in((unsigned char *)"AES P441");

          // screen->drawstring((unsigned char *)"AES P441");
          // screen->render();

          // CRGB* buf_ptr = screen->get_buffer();


          
          // uint32_t buf_len = 40*7;
          // uint8_t sine8 =1;// beatsin8(120);
          // // while(1)
          // // {
          // // while(sine8--)
          // // {
            
          // //   for (uint16_t i = 0; i < buf_len; ++i)
          // //   {
          // //       // buf_ptr[i] =  buf_ptr[i].setHue(sine8);
          // //       // hsv2rgb_raw(CHSV(255,255,sine8), buf_ptr[i]);
          // //       FastLED.setBrightness( sine8 );
          // //   }
          // //   screen->render();
          // // }

          //  while(sine8++ < 255)
          // {
            
          //   for (uint16_t i = 0; i < buf_len; ++i)
          //   {
          //       // buf_ptr[i] =  buf_ptr[i].setHue(sine8);
          //       // hsv2rgb_raw(CHSV(255,255,sine8), buf_ptr[i]);
          //        FastLED.setBrightness( sine8 );
          //   }
          //   screen->render();
          // }
          // // }
          // delay(60);
          // for (uint16_t i = 0; i < buf_len; ++i)  if (i % 2 != 0) buf_ptr[i] &= 0xAA;
          // screen->render();
          // delay(60);
          // for (uint16_t i = 0; i < buf_len; ++i)  if (i % 2 != 0) buf_ptr[i] &= 0x00;
          // screen->render();
          // delay(60);
          // for (uint16_t i = 0; i < buf_len; ++i)  if (i % 2 == 0) buf_ptr[i] &= 0x55;
          // screen->render(); 
          // delay(60);
          // for (uint16_t i = 0; i < buf_len; ++i)  if (i % 2 == 0) buf_ptr[i] &= 0x00;
          // screen->render();
          // delay(60); 
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

      callback_home();
      _newState(HOME, now);
      _buttonState = IDLE;
      _delta = 0;
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
    if (g_mute && _buttonState != CLICK)
    {
      _delta = 0;
      // _startTime = now; // once get out of mute, just go in home display();
      break;
    }

    if (waitTime > TIME_OUT && !_delta && !g_mute)
    {
      callback_home();
      _startTime = now;
    }

    // mute
    if (_buttonState == CLICK)
    {
      callback_mute();
      _buttonState = IDLE;
      _startTime = now;
      break;
    }

    if (_delta && !g_mute)
    {
      // Vol: 0~200
      callback_volume(_delta);
      _delta = 0;
      _startTime = now;
      break;
    }

    if (_buttonState == D_CLICK && !g_mute)
    {
      callback_spin_menu();
      _newState(MENU, now);
      _buttonState = IDLE;
      break;
    }

    
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
    if (_buttonState == D_CLICK)
    {
      callback_spin_cmd(_delta);
      _newState(CMD, now);
      _buttonState = IDLE;
      break;
    }

    if (_delta)
    {
      g_cur_cmd_index += _delta;
      _delta = 0;
      callback_spin_menu();
      _startTime = now;
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
    if (_buttonState == CLICK)
    {
      _newState(SET, now);
      _buttonState = IDLE;
      break;
    }

    if (_buttonState == D_CLICK)
    {
      //TODO: restore cfg and update.
      callback_spin_menu();
      _newState(MENU, now);
      _buttonState = IDLE;
      break;
    }

    if (_delta)
    {
      // Serial.printf("%d", _delta);
      callback_spin_cmd(_delta);
      _delta = 0;
      _startTime = now;
      break;
    }

    if (_buttonState == L_CLICK || waitTime > TIME_OUT)
    {
      callback_home();
      _newState(HOME, now);
      _buttonState = IDLE;
      break;
    }
    break;
  }
  case SET:
  {
  if (_buttonState == CLICK)
  {
    _newState(CMD, now);
    _buttonState = IDLE;
    break;
  }

  if (_delta)
  {
    // Serial.printf("%d", _delta);
    callback_spin_set(_delta);
    _delta = 0;
    break;
  }

  }
  
  default:
    break;
  }

}
