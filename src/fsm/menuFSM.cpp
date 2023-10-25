#include "menuFSM.h"


static MenuFsm* g_FSM = nullptr;
static MenuFsm* g_menu_crtl = nullptr;

MenuFsm* MenuFsm::getInstance()
{
    if (g_FSM == nullptr)
    {
        g_FSM = new MenuFsm();
    }
    return g_FSM;
}

void MenuFsm::callback_boot(void)
{
  Serial.println("call_back: boot");
  Display* screen = Display::getInstance();
  screen->showLogo(0);
}

void MenuFsm::callback_mute()
{
  if (_mute)
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

void MenuFsm::callback_volume(int8_t delta)
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

void MenuFsm::callback_home()
{
    // parser infos from FPGA;

    // gen mapping string;

    // show
    Display* screen = Display::getInstance();
    screen->setCharColor(100,100,100);
    screen->drawstring((unsigned char *)"AES P441");
    screen->render();
}

void MenuFsm::callback_spin_menu()
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

void MenuFsm::callback_spin_set(int8_t delta)
{

}

void MenuFsm::callback_spin_cmd(int8_t delta)
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



void MenuFsm::FSM_tick(unsigned long now)
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

    case POWEROFF:
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
      if (waitTime > menu_handle->LOADING_TIME)
      {
        _newState(HOME, now);
      }
      break;
    }

    case HOME:
    {
      if (_buttonState == CLICK)
      {
        _newState(MUTE, now);
      }
      else if(_buttonState == D_CLICK)
      {
        _newState(MENU, now);
      }
      else if (_delta)
      {
        _newState(VOL, now);
      }
      break;
    }

    case MUTE:
    {
      if (_buttonState == CLICK)
      {
        _newState(HOME, now);
      }
      break;
    }

    case VOL:
    {
      if (waitTime > menu_handle->VOL_SHOW_TIME)
      {
        _newState(HOME, now);
      }
      break;
    }

    case MENU:
    {
      if (waitTime > menu_handle->TIME_OUT || _buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      else if (_buttonState == D_CLICK)
      {
        _newState(CMD, now);
      }

      break;
    }

    case CMD:
    {
      if (waitTime > menu_handle->TIME_OUT || _buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      else if (_buttonState == D_CLICK)
      {
        _newState(SET, now);
      }

      break;
    }

    case SET:
    {
      if (waitTime > menu_handle->TIME_OUT || _buttonState == L_CLICK)
      {
        _newState(HOME, now);
      }
      else if (_buttonState == D_CLICK)
      {
        _newState(CMD, now);
      }
      break;
    }
  }
}

void MenuFsm::FSM_executor(unsigned long now)
{
  unsigned long waitTime = (now - _startTime);

  switch (_cur_state)
  {
    case POWEROFF:
    {
      if (_lastState == POWERON)
      {
        // save context and clear the screen;
        // step 1: save context;
        // TODO: eq.reset params or save params to eeprom;
        // _mute = 0;

        Display* screen = Display::getInstance();
        // waitting for FPGA ready down, then show logo 3s;
        if (get_fpga_ready() || waitTime < 3000)
        {
          screen->showLogo(0);
        }
        else
        {
          // step 2: clear the screen.
          screen->screenReset();
          // close the big lED.
        }
      }
      break;
    }

    case POWERON:
    {
      // do something before enter home page.
      if (waitTime < menu_handle->LOADING_TIME)
      {
        menu_handle->show_booting();
      }
      break;
    }

    case HOME:
    {
      if (waitTime > menu_handle->TIME_OUT)
      {
        Display* screen = Display::getInstance();
        screen->setCharColor(100,0,0);
        screen->drawstring((unsigned char *)"FPGA ERR");
        screen->render();

        // re-trying connect to FPGA......
        _fresh_starttime(now);
      }

      menu_handle->show_home();
      break;
    }

    case MUTE:
    {
      // first comsume the button state;
      _release_button();

      // TODO: volume slide in;
      menu_handle->set_voice_volume(0);

      // render the statue;
      
      Display* screen = Display::getInstance();
      screen->drawstring((unsigned char *)"  Mute  ");
      screen->render();

      // mask the spin and timeout function.
      _reset_delta(); 
      _fresh_starttime(now);
      break;
    }

    case VOL:
    {
      if (_delta)
      {
        menu_handle->update_volume(_delta);
        _reset_delta(); // product-consumer mode.
        menu_handle->show_volume();
        _fresh_starttime();
      }
      break;
    }

    case MENU:
    {
      if (_delta)
      {
        menu_hadle->spin_L1_menu(_delta);
        _reset_delta();
        _fresh_starttime(); // in case timeout go home.
      }
      else if (waitTime >= menu_handle->TIME_OUT || _buttonState == L_CLICK)
      {
        menu_handle->show_home();
        _newState(HOME, now);
        _buttonState = IDLE;
        break;
      }
      break;
    }

    case CMD:
    {


      break;
    }

    case SET:
    {

      break;
    }
  }
}
