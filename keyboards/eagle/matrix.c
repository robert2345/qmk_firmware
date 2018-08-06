#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "timer.h"
#include "wait.h"
#include "print.h"
#include "matrix.h"


/*
 * Infinity Pinusage:
 * Column pins are input with internal pull-down. Row pins are output and strobe with high.
 * Key is high or 1 when it turns on.
 *  INFINITY PRODUCTION (NO LED)
 *     col: { PTD1, PTD2, PTD3, PTD4, PTD5, PTD6, PTD7 }
 *     row: { PTB0, PTB1, PTB2, PTB3, PTB16, PTB17, PTC4, PTC5, PTD0 }
 *  INFINITY PRODUCTION (WITH LED)
 *     col: { PTD1, PTD2, PTD3, PTD4, PTD5, PTD6, PTD7 }
 *     row: { PTC0, PTC1, PTC2, PTC3, PTC4, PTC5, PTC6, PTC7, PTD0 }
 */
/*
 * Eagle Pinusage:
 * Row pins are input with internal pull-down. Col pins are output and strobe with high.
 * Key is high or 1 when it turns on.
 */

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_COLS];
static matrix_row_t matrix_debouncing[MATRIX_COLS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;

/** https://forum.pjrc.com/archive/index.php/t-17532.html
 *
 * Teensy-3 Pin, GPIO Port, GPIO Bit #
---------------
0 B 16
1 B 17
2 D 0
3 A 12
4 A 13
5 D 7
6 D 4
7 D 2
8 D 3
9 C 3
10 C 4
11 C 6
12 C 7
13 C 5
14 D 1
15 C 0
16 B 0
17 B 1
18 B 3
19 B 2
20 D 5
21 D 6
22 C 1
23 C 2
24 A 5
25 B 19
26 E 1
27 C 9
28 C 8
*/
void matrix_init(void)
{
    /* Column(strobe) */
    palSetPadMode(GPIOB, 16,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 17,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 12,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 13,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 4,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 2,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 4,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 6,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 7,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 5,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOD, 1,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOC, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 1,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOB, 3,  PAL_MODE_OUTPUT_PUSHPULL);

    /* Row(sense) */
    palSetPadMode(GPIOB, 2,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOD, 5,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOD, 6,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOC, 1,  PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOC, 2, PAL_MODE_INPUT_PULLDOWN);

    memset(matrix, 0, MATRIX_ROWS * sizeof(matrix_row_t));
    memset(matrix_debouncing, 0, MATRIX_ROWS * sizeof(matrix_row_t));

    matrix_init_quantum();
}

uint8_t matrix_scan(void)
{
    for (int col = 0; col < MATRIX_ROWS; col++) {
        matrix_row_t data = 0;
        // strobe row
        switch (col) {
		case 0: palSetPad(GPIOB, 16); break;
		case 1: palSetPad(GPIOB, 17); break;
		case 2: palSetPad(GPIOD, 0); break;
		case 3: palSetPad(GPIOA, 12); break;
		case 4: palSetPad(GPIOA, 13); break;
		case 5: palSetPad(GPIOD, 7); break;
		case 6: palSetPad(GPIOD, 4); break;
		case 7: palSetPad(GPIOD, 2); break;
		case 8: palSetPad(GPIOD, 3); break;
		case 9: palSetPad(GPIOC, 3); break;
		case 10: palSetPad(GPIOC, 4); break;
		case 11: palSetPad(GPIOC, 6); break;
		case 12: palSetPad(GPIOC, 7); break;
		case 13: palSetPad(GPIOC, 5); break;
		case 14: palSetPad(GPIOD, 1); break;
		case 15: palSetPad(GPIOC, 0); break;
		case 16: palSetPad(GPIOB, 0); break;
		case 17: palSetPad(GPIOB, 1); break;
		case 18: palSetPad(GPIOB, 3); break;
        }

        // need wait to settle pin state
        // if you wait too short, or have a too high update rate
        // the keyboard might freeze, or there might not be enough
        // processing power to something fancy. Whatever that might be in the future.
	
	wait_us(20);

	 // read row data: { PTD1, PTD4, PTD5, PTD6, PTD7 }
        data = (palReadPad(GPIOB, 2) |
             (palReadPad(GPIOD, 6) << 1) |
              (palReadPad(GPIOD, 6) << 2) |
              (palReadPad(GPIOC, 1) << 3) |
              (palReadPad(GPIOC, 2) << 4));
	
        // un-strobe col
        switch (col) {
		case 0: palClearPad(GPIOB, 16); break;
		case 1: palClearPad(GPIOB, 17); break;
		case 2: palClearPad(GPIOD, 0); break;
		case 3: palClearPad(GPIOA, 12); break;
		case 4: palClearPad(GPIOA, 13); break;
		case 5: palClearPad(GPIOD, 7); break;
		case 6: palClearPad(GPIOD, 4); break;
		case 7: palClearPad(GPIOD, 2); break;
		case 8: palClearPad(GPIOD, 3); break;
		case 9: palClearPad(GPIOC, 3); break;
		case 10: palClearPad(GPIOC, 4); break;
		case 11: palClearPad(GPIOC, 6); break;
		case 12: palClearPad(GPIOC, 7); break;
		case 13: palClearPad(GPIOC, 5); break;
		case 14: palClearPad(GPIOD, 1); break;
		case 15: palClearPad(GPIOC, 0); break;
		case 16: palClearPad(GPIOB, 0); break;
		case 17: palClearPad(GPIOB, 1); break;
		case 18: palClearPad(GPIOB, 3); break;
        }

        if (matrix_debouncing[col] != data) {
            matrix_debouncing[col] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            matrix[col] = matrix_debouncing[col];
        }
        debouncing = false;
    }
    matrix_scan_quantum();
    return 1;
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    xprintf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%02X: ");
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}
