#include "nyquist.h"
#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#include "quantum_keycodes.h"
#include "pincontrol.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <Arduino.h>


#define _______ KC_TRNS

#define SOLENOID_DEFAULT_DWELL 75 
#define SOLENOID_MAX_DWELL 100
#define SOLENOID_MIN_DWELL 4
#define SOLENOID_PIN D3

#define TIMER3 3

extern keymap_config_t keymap_config;

enum planck_keycodes {
  SOLENOID_TOG = SAFE_RANGE,
  SOLENOID_DWELL_MINUS,
  SOLENOID_DWELL_PLUS,
  SOLENOID_BUZZ_ON,
  SOLENOID_BUZZ_OFF,
  EXT_PLV
};

int timer3_pin_port;
int timer3_pin_mask;

bool solenoid_enabled = false;
bool solenoid_on = false;
bool solenoid_buzz = false;
bool solenoid_buzzing = false;
uint16_t solenoid_start = 0;
uint8_t solenoid_dwell = SOLENOID_DEFAULT_DWELL;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    KEYMAP(
            KC_GRAVE,  KC_1,     KC_2,     KC_3,     KC_4,   KC_5,    KC_6,    KC_7,   KC_8,     KC_9,     KC_0,     KC_BSPC,
            KC_TAB,    KC_Q,     KC_W,     KC_E,     KC_R,   KC_T,    KC_Y,    KC_U,   KC_I,     KC_O,     KC_P,     KC_LBRC,
            KC_ESC,    KC_A,     KC_S,     KC_D,     KC_F,   KC_G,    KC_H,    KC_J,   KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
            KC_LSFT,   KC_Z,     KC_X,     KC_C,     KC_V,   KC_B,    KC_N,    KC_M,   KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,
            MO(3),     KC_LCTL,  KC_LALT,  KC_LGUI,  MO(1),  KC_ENT,  KC_SPC,  MO(2),  KC_BSLS,  KC_RALT,  KC_RCTL,  _______),

    KEYMAP(
            KC_F12,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,    KC_F11,
            _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_INS,   KC_HOME,  KC_PGUP,  _______,   _______,
            _______,  _______,  _______,  _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_MINUS,  KC_EQUAL,
            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_END,   KC_PGDN,  _______,   _______,
            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______),

    KEYMAP(
            KC_F12,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,
            _______,  _______,  _______,  KC_LBRC,  KC_RBRC,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
            _______,  KC_CIRC,  _______,  KC_LPRN,  KC_RPRN,  KC_DLR,   _______,  _______,  _______,  _______,  _______,  _______,
            _______,  _______,  _______,  KC_LCBR,  KC_RCBR,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______),

    KEYMAP(
            RESET,    _______,  _______,  _______,  _______,  _______,  SOLENOID_TOG,         _______,  KC_SLSH,  KC_PAST,  KC_PMNS,        KC_DEL,
            _______,  _______,  _______,  _______,  _______,  _______,  _______/*CLICKY_TOGGLE*/,  KC_7,     KC_8,     KC_9,     KC_PLUS,        _______,
            _______,  _______,  _______,  _______,  _______,  _______,  AU_TOG,         KC_4,     KC_5,     KC_6,     LSFT(KC_SCLN),  KC_EQUAL,
            _______,  _______,  _______,  _______,  _______,  _______,  MU_TOG,         KC_1,     KC_2,     KC_3,     KC_ENT,         _______,
            _______,  _______,  _______,  _______,  _______,  _______,  _______,        KC_0,     KC_0,     KC_DOT,   KC_ENT,         KC_CAPS),
};

/* Local function prototypes */
static void enable_timer();
static void setup_timer();

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  return MACRO_NONE;
}

void solenoid_buzz_on(void) {
  solenoid_buzz = true;
}

void solenoid_buzz_off(void) {
  solenoid_buzz = false;
}

void solenoid_dwell_minus(void) {
  if (solenoid_dwell > 0) solenoid_dwell--;
}

void solenoid_dwell_plus(void) {
  if (solenoid_dwell < SOLENOID_MAX_DWELL) solenoid_dwell++;
}

void solenoid_toggle(void) {
  solenoid_enabled = !solenoid_enabled;
} 

void solenoid_stop(void) {
  digitalWrite(SOLENOID_PIN, PinLevelLow);
  solenoid_on = false;
  //solenoid_buzzing = false;
}

void solenoid_check(void) {
  uint16_t elapsed = 0;

  if (!solenoid_on) return;

  elapsed = timer_elapsed(solenoid_start);

  //Check if it's time to finish this solenoid click cycle 
  if (elapsed > solenoid_dwell) {
    solenoid_stop();
    return;
  }

  //Check whether to buzz the solenoid on and off
  if (solenoid_buzz) {
    if (elapsed / SOLENOID_MIN_DWELL % 2 == 0){
      if (!solenoid_buzzing) {
        solenoid_buzzing = true;
        digitalWrite(SOLENOID_PIN, PinLevelHigh);
      }
    }
    else {
      if (solenoid_buzzing) {
        solenoid_buzzing = false;
        digitalWrite(SOLENOID_PIN, PinLevelLow);
      }
    }
  }
  
}

void solenoid_fire(void) {
    /*
  if (!solenoid_enabled) return;

  if (!solenoid_buzz && solenoid_on) {
     solenoid_check();
     return;
  }

  if (solenoid_buzz && solenoid_buzzing) return;
*/

    //timer variant
    if (solenoid_on) return;

  solenoid_on = true;
  //solenoid_buzzing = true;
  enable_timer();
  solenoid_start = timer_read(); 
  digitalWrite(SOLENOID_PIN, PinLevelHigh);
}

void solenoid_setup(void) {
  pinMode(SOLENOID_PIN, PinDirectionOutput);
  setup_timer();
}

void matrix_init_user(void) {
  solenoid_setup();
}

void matrix_scan_user(void) {
  //solenoid_check();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 
  if (record->event.pressed) {
    solenoid_fire(); 
  }

  switch (keycode) {
    /*case SING_RANDOM_SONG:
      if (record->event.pressed) {
        sing_random_song();
      }
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
        PORTE &= ~(1<<6);
      } else {
        unregister_code(KC_RSFT);
        PORTE |= (1<<6);
      }
      return false;
      break;*/
    case SOLENOID_TOG:
      if (record->event.pressed) {
        solenoid_toggle();
      }
      break;
    case SOLENOID_DWELL_MINUS:
      if (record->event.pressed) {
        solenoid_dwell_minus();
      }
      break;
    case SOLENOID_DWELL_PLUS:
      if (record->event.pressed) {
        solenoid_dwell_plus();
      }
      break;
    case SOLENOID_BUZZ_ON:
      if (record->event.pressed) {
        solenoid_buzz_on();
      }
      break;
    case SOLENOID_BUZZ_OFF:
      if (record->event.pressed) {
        solenoid_buzz_off();
      }
      break;
  }
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

static void setup_timer() {
    // 8 bit timer
    TCCR3A = 0;
    TCCR3B = 0;
    bitWrite(TCCR3A, WGM31, 1);
    bitWrite(TCCR3B, CS30, 1);
    timer3_pin_port = portOutputRegister(digitalPinToPort(SOLENOID_PIN));
    timer3_pin_mask = digitalPinToBitMask(SOLENOID_PIN);
}

static void enable_timer()
{
  uint8_t prescalarbits = 0b001;
  //long toggle_count = 0;
  uint32_t ocr = 0;
  int8_t _timer = TIMER3;

  int frequency = 20;

  ocr = F_CPU / frequency / 2 - 1;
  prescalarbits = 0b001;  // ck/1: same for both timers
  if (ocr > 255)
  {
      ocr = F_CPU / frequency / 2 / 8 - 1;
      prescalarbits = 0b010;  // ck/8: same for both timers

      if (_timer == 2 && ocr > 255)
      {
          ocr = F_CPU / frequency / 2 / 32 - 1;
          prescalarbits = 0b011;
      }

      if (ocr > 255)
      {
          ocr = F_CPU / frequency / 2 / 64 - 1;
          prescalarbits = _timer == 0 ? 0b011 : 0b100;

          if (_timer == 2 && ocr > 255)
          {
              ocr = F_CPU / frequency / 2 / 128 - 1;
              prescalarbits = 0b101;
          }

          if (ocr > 255)
          {
              ocr = F_CPU / frequency / 2 / 256 - 1;
              prescalarbits = _timer == 0 ? 0b100 : 0b110;
              if (ocr > 255)
              {
                  // can't do any better than /1024
                  ocr = F_CPU / frequency / 2 / 1024 - 1;
                  prescalarbits = _timer == 0 ? 0b101 : 0b111;
              }
          }
      }
  }

  TCCR3B = prescalarbits;

  // Set the OCR for the given timer,
  // set the toggle count,
  // then turn on the interrupts
#if defined(OCR2A) && defined(TIMSK2) && defined(OCIE2A)
  OCR3A = ocr;
  bitWrite(TIMSK3, OCIE3A, 1);
#endif

}


void disableTimer(uint8_t _timer)
{
    bitWrite(TIMSK3, OCIE3A, 0); // disable interrupt
    TCCR3A = (1 << WGM30);
    TCCR3B = (TCCR3B & 0b11111000) | (1 << CS32);
    OCR3A = 0;
}

ISR(TIMER3_COMPA_vect)
{
 solenoid_stop();
 disableTimer(TIMER3);
}


