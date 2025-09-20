#include <avr/io.h>
#include <util/delay.h>
#include "gridfinding_def.h"

// Keypad mapping
static const char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

typedef enum InputState {
    IDLE,
    PICKUP_X,
    PICKUP_Y,
    DROPOFF_X,
    DROPOFF_Y,
    PICKUP_X2,
    PICKUP_Y2,
    DROPOFF_X2,
    DROPOFF_Y2,

} InputState;

InputState inputState = IDLE;


// Positiebepaling
void xNuFinder(void) {
    if (PIN_pos_XY & (1 << pos_X1)) xNu = 1;
    if (PIN_pos_XY & (1 << pos_X2)) xNu = 2;
    if (PIN_pos_XY & (1 << pos_X3)) xNu = 3;
    if (PIN_pos_XY & (1 << pos_X4)) xNu = 4;
    if (PIN_pos_XY & (1 << pos_X5)) xNu = 5;
}

void yNuFinder(void) {
    if (PIN_pos_XY & (1 << pos_Y1)) yNu = 1;
    if (PIN_pos_XY & (1 << pos_Y2)) yNu = 2;
    if (PIN_pos_XY & (1 << pos_Y3)) yNu = 3;
    if (PIN_pos_Y  & (1 << pos_Y4)) yNu = 4;
    if (PIN_pos_Y  & (1 << pos_Y5)) yNu = 5;
}

// Keypad uitlezen
char keypad_getkey(void) {
    for (int r = 0; r < ROWS; r++) {
        ROW_PORT = (1 << r);
        _delay_us(5);

        for (int c = 0; c < COLS; c++) {
            if (!(COL_PIN & (1 << c))) {
                while (!(COL_PIN & (1 << c))); // wachten tot losgelaten
                _delay_ms(50);                 // debounce
                return keys[r][c];
            }
        }
    }
    return 0;
}

void processKey(char key) {
    switch (inputState) {
        case IDLE:
            if (key == 'A') {
                inputState = PICKUP_X;
            } else if (key == 'B') {
                inputState = DROPOFF_X;
            } else if ((key == 'C') && (PIN_SwitchTweedeCoord & (1 << pinSwitchTweedeCoord))) {
                inputState = PICKUP_X2;
            } else if ((key == 'D') && (PIN_SwitchTweedeCoord & (1 << pinSwitchTweedeCoord))) {
                inputState = DROPOFF_X2;
            }
            if((infoEindPosOpgehaald == 1) && (infoEindPosOpgehaald2 == 1)){
                break;
            }

            break;

        case PICKUP_X:
            if (key >= '1' && key <= '5') {
                xEind = key - '0';
                inputState = PICKUP_Y;
            } else {
                infoEindPosOpgehaald = 0;
                inputState = IDLE;  // wrong key cancels
            }
            break;

        case PICKUP_Y:
            if (key >= '1' && key <= '5') {
                yEind = key - '0';
                infoEindPosOpgehaald = 1;
                inputState = IDLE;
            } else {
                infoEindPosOpgehaald = 0;
                inputState = IDLE;  // wrong key cancels
            }
            break;

        case DROPOFF_X:
            if (key >= '1' && key <= '5') {
                xEindDropOf = key - '0';
                inputState = DROPOFF_Y;
            } else {
                infoEindPosOpgehaald = 0;
                inputState = IDLE;
            }
            break;

        case DROPOFF_Y:
            if (key >= '1' && key <= '5') {
                yEindDropOf = key - '0';
                infoEindPosOpgehaald = 1;
                inputState = IDLE;
            } else {
                infoEindPosOpgehaald = 0;
                inputState = IDLE;
            }
            break;

        case PICKUP_X2:
            if (key >= '1' && key <= '5') {
                xEind2 = key - '0';
                inputState = PICKUP_Y2;
            } else {
                infoEindPosOpgehaald2 = 0;
                inputState = IDLE;  // wrong key cancels
            }
            break;

        case PICKUP_Y2:
            if (key >= '1' && key <= '5') {
                yEind2 = key - '0';
                infoEindPosOpgehaald2 = 1;
                inputState = IDLE;
            } else {
                infoEindPosOpgehaald2 = 0;
                inputState = IDLE;  // wrong key cancels
            }
            break;

        case DROPOFF_X2:
            if (key >= '1' && key <= '5') {
                xEindDropOf2 = key - '0';
                inputState = DROPOFF_Y2;
            } else {
                infoEindPosOpgehaald2 = 0;
                inputState = IDLE;
            }
            break;

        case DROPOFF_Y2:
            if (key >= '1' && key <= '5') {
                yEindDropOf2 = key - '0';
                infoEindPosOpgehaald2 = 1;
                inputState = IDLE;
            } else {
                infoEindPosOpgehaald2 = 0;
                inputState = IDLE;
            }
            break;
    }
}

void pickUp_and_DropOff_pos(void) {
    while (1) {
        if((infoEindPosOpgehaald == 1) && (infoEindPosOpgehaald2 == 1)){
                startSlot = 1;
                break;
            }

        char key = keypad_getkey();
        if (key) {
            processKey(key);
        }
    }
}
