#include "nyquist.h"
#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"

#define _______ KC_TRNS

extern keymap_config_t keymap_config;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  KEYMAP(
      KC_GRAVE,  KC_1,     KC_2,     KC_3,     KC_4,   KC_5,    KC_6,    KC_7,   KC_8,     KC_9,     KC_0,     KC_BSPC,
      KC_TAB,    KC_Q,     KC_W,     KC_E,     KC_R,   KC_T,    KC_Y,    KC_U,   KC_I,     KC_O,     KC_P,     KC_BSLS,
      KC_ESC,    KC_A,     KC_S,     KC_D,     KC_F,   KC_G,    KC_H,    KC_J,   KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
      KC_LSFT,   KC_Z,     KC_X,     KC_C,     KC_V,   KC_B,    KC_N,    KC_M,   KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
      MO(3),     KC_LCTL,  KC_LALT,  KC_LGUI,  MO(1),  KC_ENT,  KC_SPC,  MO(2),  KC_BSLS,  KC_RALT,  KC_RCTL,  _______),

  KEYMAP(
      KC_F12,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_INS,   KC_HOME,  KC_PGUP,  _______,  KC_LBRC,
      _______,  _______,  _______,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_END,   KC_PGDN,  _______,  _______,
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______),

  KEYMAP(
      KC_F12,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
      _______,  _______,  _______,  KC_LBRC,  KC_RBRC,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
      _______,  KC_CIRC,  _______,  KC_LPRN,  KC_RPRN,  KC_DLR,   _______,  _______,  _______,  _______,  _______,  _______,
      _______,  _______,  _______,  KC_LCBR,  KC_RCBR,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______),

  KEYMAP(
      RESET,    _______,  _______,  _______,  _______,  _______,  _______,  KC_LNUM,  KC_SLSH,  KC_PAST,  KC_PMNS,     KC_DEL,
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_7,     KC_8,     KC_9,     KC_PLUS,     RALT(KC_W),
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_4,     KC_5,     KC_6,     RALT(KC_P),  RALT(KC_Q),
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_1,     KC_2,     KC_3,     KC_ENT,      _______,
      _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_0,     KC_0,     KC_DOT,   KC_ENT,      KC_CAPS),

};


const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  return MACRO_NONE;
}

void matrix_init_user(void) {
}

void matrix_scan_user(void) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  return true;
}

void led_set_user(uint8_t usb_led) {

  if (usb_led & (1 << USB_LED_NUM_LOCK)) {

  } else {

  }

  if (usb_led & (1 << USB_LED_CAPS_LOCK)) {

  } else {

  }

  if (usb_led & (1 << USB_LED_SCROLL_LOCK)) {

  } else {

  }

  if (usb_led & (1 << USB_LED_COMPOSE)) {

  } else {

  }

  if (usb_led & (1 << USB_LED_KANA)) {

  } else {

  }

}

