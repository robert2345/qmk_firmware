#include "eagle.h"

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP(
    KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_TAB, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_LSFT, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    _______, _______, _______, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, _______, _______, _______),

    [1] = KEYMAP(
    KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_TAB, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    KC_LSFT, KC_1, KC_2, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
    _______, _______, _______, KC_3, KC_4, KC_5, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_LSFT, KC_6, KC_7, KC_8, _______, _______, _______)
};

const uint16_t fn_actions[] = {
    
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

};
