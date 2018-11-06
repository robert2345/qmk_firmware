#include "milvus.h"

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,  KC_5,   KC_INS,  KC_HOME, KC_PGDN, _______, KC_SLSH, KC_PAST, KC_PMNS, KC_6,   KC_7,  KC_8,     KC_9,    KC_0,    KC_BSPC,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,  KC_T,   KC_DEL,  KC_END,  KC_PGUP, KC_7,    KC_8,    KC_9,    KC_PLUS, KC_Y,   KC_U,  KC_I,     KC_O,    KC_P,    KC_LBRC,
    KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,  KC_G,   _______, KC_UP,   _______, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_H,   KC_J,  KC_K,     KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,  KC_B,   KC_LEFT, KC_DOWN, KC_RGHT, KC_1,    KC_2,    KC_3,    KC_ENT,  KC_N,   KC_M,  KC_COMM,  KC_DOT,  KC_SLSH, KC_RSFT,
    _______, _______, _______, KC_LCTL, MO(1), KC_ENT, KC_WIN,  KC_LALT, _______, KC_0,    KC_0,    KC_DOT,  KC_ENT,  KC_SPC, MO(1), KC_RCTRL, _______, _______, _______),

    [1] = KEYMAP(
    KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______, _______, _______, _______, _______, _______, _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,
    _______, _______, _______, KC_LBRC, KC_RBRC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
    _______, KC_CIRC, _______, KC_LPRN, KC_RPRN, KC_DLR,  _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MINUS, KC_EQUAL,
    _______, _______, _______, KC_LCBR, KC_RCBR, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______)
};

const uint16_t fn_actions[] = {
    
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};
static long counter = 0;
static bool active = true;
// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
	counter++;
	if (counter == 0) active = !active;
	
	if (active) palSetPad(GPIOC, 5);
	else palClearPad(GPIOC, 5);

};
