/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "menu.h"
#include "rig.h"
#include "cat_task.h"
#include "display_task.h"
#include "keyer_task.h"
#include "objs.h"

// menu mode
bool select_menu_mode(int16_t val, bool selected) {
  if (!selected) return true;

  switch (val) {
  case 0:
    rig.setMode(MODE_CW);
    break;
  case 1:
    rig.setMode(MODE_CWR);
    break;
  case 2:
    rig.setMode(MODE_LSB);
    break;
  case 3:
    rig.setMode(MODE_USB);
    break;
  default:
    break;
  }

  return true;
}

void format_menu_value_mode(char *buf, int16_t val) {
  switch (val) {
  case 0:
    strcpy_P(buf, PSTR("CW "));
    break;
  case 1:
    strcpy_P(buf, PSTR("CWR"));
    break;
  case 2:
    strcpy_P(buf, PSTR("LSB"));
    break;
  case 3:
    strcpy_P(buf, PSTR("USB"));
    break;
  default:
    strcpy_P(buf, PSTR("N/A"));
    break;
  }
}

int16_t get_menu_value_mode() {
  int16_t result = -1;

  switch (rig.getMode()) {
  case MODE_CW:
    result = 0;
    break;
  case MODE_CWR:
    result = 1;
    break;
  case MODE_LSB:
    result = 2;
    break;
  case MODE_USB:
    result = 3;
    break;
  default:
    break;
  }

  return result;
}

// menu A/B
bool select_menu_exchange_vfo(int16_t, bool) {
  rig.exchangeVfo(false);
  return true;
}

// menu A=B
bool select_menu_equalize_vfo(int16_t, bool) {
  rig.equalizeVfo(false);
  return true;
}

// menu split
bool select_menu_split(int16_t, bool) {
  rig.setSplit(rig.getSplit() == ON ? OFF : ON, false);
  return true;
}

void format_menu_split(char *buf, const char *, bool, int16_t) {
  strcpy_P(buf, rig.getSplit() == ON ? PSTR("Split OFF") : PSTR("Split ON"));
}

// menu V/M
bool select_menu_vm(int16_t, bool) {
  int8_t ch;
  if (rig.isVfo()) {
    if (!rig.isMemOk()) {
      ch = rig.getNextMemOkCh(rig.getMemCh());
      if (ch != -1) {
        rig.selectMemCh(ch, false);
        rig.selectMem(false);
      }
    }
    rig.selectMem(false);
  } else {
    rig.selectVfo(false);
  }

  return true;
}

// menu M->V, MW, MC
bool select_menu_mem_to_vfo(int16_t val, bool selected) {
  if (!selected) return true;

  rig.selectMemCh(val, false);
  rig.memoryToVfo(val, false);
  rig.selectVfo(false);

  return true;
}

bool select_menu_mem_write(int16_t val, bool selected) {
  if (!selected) return true;

  rig.selectMemCh(val, false);
  rig.writeMemory(val, false);

  return true;
}

bool select_menu_mem_clear(int16_t val, bool selected) {
  if (!selected) return true;

  rig.selectMemCh(val, false);
  rig.clearMemory(false);

  return true;
}

int16_t get_menu_value_mem_ch() {
  return rig.getMemCh();
}

int16_t get_menu_value_mem_ok_ch() {
  int16_t result;
  if (rig.isMemOk()) {
    result = rig.getMemCh();
  } else {
    result = rig.getNextMemOkCh(rig.getMemCh());
  }

  return result;
}

void format_menu_value_mem_ch(char *buf, int16_t val) {
  if (val >= 0)
    sprintf(buf, rig.isMemOk(val) ? "M%02d" : "?%02d", val);
  else
    sprintf(buf, "N/A");
}

int16_t get_next_menu_value_mem_ok_ch(int16_t val, bool forward) {
  int16_t result;

  if (forward) result = rig.getNextMemOkCh(val);
  else result = rig.getPrevMemOkCh(val);

  return result;
}

void format_menu_no_val(char *buf, const char *original_text, bool, int16_t) {
  sprintf(buf, "%s", original_text);
}

void format_menu_value_yes_no(char *buf, int16_t val) {
  sprintf(buf, val == 0 ? "No " : "Yes");
}

int16_t get_menu_value_no() {
  return 0;
}

bool select_menu_cw_tone(int16_t val, bool selected) {
  if (!selected) return true;

  Device::setCwTone(val * 50 + 400);

  return true;
}

int16_t get_menu_value_cw_tone() {
  int16_t result = (Device::getCwTone() - 400) / 50;
  if (result < 0) result = 0;
  else if (result > 32) result = 32;

  return result;
}

void format_menu_value_cw_tone(char *buf, int16_t val) {
  uint16_t cwTone = val * 50 + 400;

  sprintf(buf, "%4" PRIu16, cwTone);
}

bool select_menu_cw_wpm(int16_t val, bool selected) {
  if (!selected) return true;

  Device::setCwWpm(val + 5);

  return true;
}

int16_t get_menu_value_cw_wpm() {
  int16_t result = Device::getCwWpm() - 5;
  if (result < 0) result = 0;
  else if (result > 55) result = 55;

  return result;
}

void format_menu_value_cw_wpm(char *buf, int16_t val) {
  uint16_t cwWpm = val + 5;

  sprintf(buf, "%2" PRIu16, cwWpm);
}

bool select_menu_cw_delay(int16_t val, bool selected) {
  if (!selected) return true;

  Device::setCwDelay(val * 100);

  return true;
}

int16_t get_menu_value_cw_delay() {
  int16_t result = Device::getCwDelay() / 100;
  if (result < 0) result = 0;
  else if (result > 10) result = 10;

  return result;
}

void format_menu_value_cw_delay(char *buf, int16_t val) {
  uint16_t cwDelay = val * 100;

  sprintf(buf, "%4" PRIu16, cwDelay);
}

bool select_menu_cw_key(int16_t val, bool selected) {
  if (!selected) return true;

  Device::setCwKey(val);

  return true;
}

int16_t get_menu_value_cw_key() {
  return Device::getCwKey();
}

void format_menu_value_cw_key(char *buf, int16_t val) {
  switch (val) {
  case CW_KEY_STRAIGHT:
    strcpy_P(buf, PSTR("STRAIGHT "));
    break;
  case CW_KEY_IAMBIC_A_L:
    strcpy_P(buf, PSTR("IAMBIC AL"));
    break;
  case CW_KEY_IAMBIC_A_R:
    strcpy_P(buf, PSTR("IAMBIC AR"));
    break;
  case CW_KEY_IAMBIC_B_L:
    strcpy_P(buf, PSTR("IAMBIC BL"));
    break;
  case CW_KEY_IAMBIC_B_R:
    strcpy_P(buf, PSTR("IAMBIC BR"));
    break;
  default:
    sprintf(buf, "N/A");
    break;
  }
}

bool select_menu_itu_rgn(int16_t val, bool selected) {
  if (!selected) return true;

  rig.setItuRegion(val + 1);

  return true;
}

int16_t get_menu_value_itu_rgn() {
  return rig.getItuRegion() - 1;
}

void format_menu_value_itu_rgn(char *buf, int16_t val) {
  sprintf(buf, "%" PRIi16, val + 1);
}

static bool in_calibrating = false;
static int32_t prev_calibration = 0;
static uint32_t prev_usbCarrier = 0;

bool select_menu_10m(int16_t /*val*/, bool selected) {
  in_calibrating = false;

  if (!selected) calibration = prev_calibration;

  Device::stopCalibrate10M(selected);

  displayTask.clear1();
  displayTask.print1(selected ? F("Cali Done!") : F("Cancalled!"));

  return false;
}

void format_menu_10m(char *buf, const char */*original_text*/, bool change_val, int16_t) {
  if (!change_val) {
    strcpy_P(buf, PSTR("10MHz Calibrat"));
  } else {
    strcpy_P(buf, PSTR("Cali"));
  }
}

int16_t get_menu_value_10m() {
  return calibration / 875;
}

void format_menu_value_10m(char *buf, int16_t val) {
  sprintf(buf, "%6" PRIi16, val);

  if (!in_calibrating) {
    in_calibrating = true;

    prev_calibration = calibration;

    Device::startCalibrate10M();

    displayTask.clear1();
    displayTask.print1(F("Calibrating"));
  } else {
    char msg[17];

    calibration = 875L * (int32_t)val;
    Device::updateCalibrate10M();

    sprintf(msg, "cal:%" PRIi32, calibration);
    displayTask.clear1();
    displayTask.print1(msg);
  }
}

bool select_menu_0beat(int16_t /*val*/, bool selected) {
  in_calibrating = false;

  if (!selected) calibration = prev_calibration;

  usbCarrier = prev_usbCarrier;
  Device::stopCalibrate0beat(selected);

  displayTask.clear1();
  displayTask.print1(selected ? F("Cali Done!") : F("Cancalled!"));

  return false;
}

void format_menu_0beat(char *buf, const char */*original_text*/, bool change_val, int16_t) {
  if (!change_val) {
    strcpy_P(buf, PSTR("0-Beat Calibra"));
  } else {
    strcpy_P(buf, PSTR("Cali"));
  }
}

int16_t get_menu_value_0beat() {
  return calibration / 875;
}

void format_menu_value_0beat(char *buf, int16_t val) {
  sprintf(buf, "%6" PRIi16, val);

  if (!in_calibrating) {
    in_calibrating = true;

    prev_calibration = calibration;
    prev_usbCarrier = usbCarrier;

    Device::startCalibrate0beat();

    displayTask.clear1();
    displayTask.print1(F("Calibrating"));
  } else {
    char msg[17];

    calibration = 875L * (int32_t)val;
    usbCarrier = prev_usbCarrier + 12 * ((calibration - prev_calibration) / 875L);
    Device::updateCalibrate0beat();

    sprintf(msg, "cal:%" PRIi32, calibration);
    displayTask.clear1();
    displayTask.print1(msg);
  }
}

bool select_menu_bfo(int16_t /*val*/, bool selected) {
  in_calibrating = false;

  if (!selected) usbCarrier = prev_usbCarrier;

  Device::stopCalibrateBfo(selected);

  displayTask.clear1();
  displayTask.print1(selected ? F("Cali Done!") : F("Cancalled!"));

  return false;
}

void format_menu_bfo(char *buf, const char */*original_text*/, bool change_val, int16_t) {
  if (!change_val) {
    strcpy_P(buf, PSTR("BFO Calibrate"));
  } else {
    strcpy_P(buf, PSTR("BFO"));
  }
}

int16_t get_menu_value_bfo() {
  return (usbCarrier - 11995000L) / 10;
}

void format_menu_value_bfo(char *buf, int16_t val) {
  sprintf(buf, "%6" PRIi16, val);

  if (!in_calibrating) {
    in_calibrating = true;

    prev_usbCarrier = usbCarrier;

    Device::startCalibrateBfo();

    displayTask.clear1();
    displayTask.print1(F("Calibrating"));
  } else {
    char msg[17];

    usbCarrier = ((int32_t)val) * 10 + 11995000L;
    Device::updateCalibrateBfo();

    uint8_t mode = rig.getRxMode();

    sprintf(msg, mode == MODE_CW || mode == MODE_CWR ? "CW:%" PRIu32 : "SSB:%" PRIu32, usbCarrier);
    displayTask.clear1();
    displayTask.print1(msg);
  }
}

bool select_menu_sys_conf(int16_t val, bool selected) {
  if (!selected) return false;

  if (val == 1) uiTask.gotoSysMenu();

  return true;
}

bool select_menu_rst_all(int16_t val, bool selected) {
  if (!selected) return false;

  if (val == 1) rig.resetAll();

  return false;
}

bool select_menu_sys_exit(int16_t, bool) {
  active_main_menu();

  return true;
}

const Menu_Item main_menu[] PROGMEM = {
// text submenu_count  select_menu_f             format_menu_f       get_menu_value_f          format_menu_value_f         get_next_menu_value_f
  {"Mode",          4, select_menu_mode,         NULL,               get_menu_value_mode,      format_menu_value_mode,     NULL                         },
  {"A/B",           0, select_menu_exchange_vfo, NULL,               NULL,                     NULL,                       NULL                         },
  {"A=B",           0, select_menu_equalize_vfo, NULL,               NULL,                     NULL,                       NULL                         },
  {"Split",         0, select_menu_split,        format_menu_split,  NULL,                     NULL,                       NULL                         },
  {"V/M",           0, select_menu_vm,           NULL,               NULL,                     NULL,                       NULL                         },
  {"M\x7eV", MEM_SIZE, select_menu_mem_to_vfo,   NULL,               get_menu_value_mem_ok_ch, format_menu_value_mem_ch,   get_next_menu_value_mem_ok_ch},
  {"MW",     MEM_SIZE, select_menu_mem_write,    NULL,               get_menu_value_mem_ch,    format_menu_value_mem_ch,   NULL                         },
  {"MC",     MEM_SIZE, select_menu_mem_clear,    NULL,               get_menu_value_mem_ok_ch, format_menu_value_mem_ch,   get_next_menu_value_mem_ok_ch},
  {"CW Tone",      33, select_menu_cw_tone,      NULL,               get_menu_value_cw_tone,   format_menu_value_cw_tone,  NULL                         },
  {"CW WPM",       56, select_menu_cw_wpm,       NULL,               get_menu_value_cw_wpm,    format_menu_value_cw_wpm,   NULL                         },
  {"CW Delay",     11, select_menu_cw_delay,     NULL,               get_menu_value_cw_delay,  format_menu_value_cw_delay, NULL                         },
  {"Key",           5, select_menu_cw_key,       NULL,               get_menu_value_cw_key,    format_menu_value_cw_key,   NULL                         },
  {"ITU Rgn",       3, select_menu_itu_rgn,      NULL,               get_menu_value_itu_rgn,   format_menu_value_itu_rgn,  NULL                         },
  {"SYS CONF",      2, select_menu_sys_conf,     format_menu_no_val, get_menu_value_no,        format_menu_value_yes_no,   NULL                         },
  {"Exit Menu",     0, NULL,                     NULL,               NULL,                     NULL,                       NULL                         }
};

const uint8_t main_menu_item_count = sizeof(main_menu) / sizeof(main_menu[0]);

const Menu_Item *menu = &main_menu[0];
uint8_t menu_item_count = main_menu_item_count;

const Menu_Item system_menu[] PROGMEM = {
// text submenu_count  select_menu_f         format_menu_f       get_menu_value_f         format_menu_value_f         get_next_menu_value_f
  {"Exit Menu",     0, select_menu_sys_exit, NULL,               NULL,                    NULL,                       NULL},
  {"10MHz Cal",    -1, select_menu_10m,      format_menu_10m,    get_menu_value_10m,      format_menu_value_10m,      NULL},
  {"0BEAT Cal",    -1, select_menu_0beat,    format_menu_0beat,  get_menu_value_0beat,    format_menu_value_0beat,    NULL},
  {"BFO Cal",      -1, select_menu_bfo,      format_menu_bfo,    get_menu_value_bfo,      format_menu_value_bfo,      NULL},
  {"Reset All",     2, select_menu_rst_all,  format_menu_no_val, get_menu_value_no,       format_menu_value_yes_no,   NULL}
};

const uint8_t system_menu_item_count = sizeof(system_menu) / sizeof(system_menu[0]);

void active_main_menu() {
  menu = &main_menu[0];
  menu_item_count = main_menu_item_count;

  catTask.setDisabled(false);
  keyerTask.setDisabled(false);
}

void active_system_menu() {
  menu = &system_menu[0];
  menu_item_count = system_menu_item_count;

  catTask.setDisabled(true);
  keyerTask.setDisabled(true);
}

