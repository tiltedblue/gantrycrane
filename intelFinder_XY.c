#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "gridfinding_def.h"

const uint8_t rowPins[ROWS] = {0, 1, 2, 3}; // PD0–PD3
const uint8_t colPins[COLS] = {0, 1, 2, 3}; // PC0–PC3


// Keypad mapping
static const char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

enum InputState {
    IDLE,
    PICKUP_X,
    PICKUP_Y,
    DROPOFF_X,
    DROPOFF_Y,
    PICKUP_X2,
    PICKUP_Y2,
    DROPOFF_X2,
    DROPOFF_Y2,

};

enum InputState inputState = IDLE;


// Positiebepaling
void xNuFinder(void) {
    if ((PIN_pos_XY & (1 << pos_X1)) == 0) {
        xNu = 1;
        printf("xNu_1\n");
    }
    if ((PIN_pos_XY & (1 << pos_X2)) == 0) {
        xNu = 2;
        printf("xNu_2\n");
    }
    if ((PIN_pos_XY & (1 << pos_X3)) == 0) {
        xNu = 3;
        printf("xNu_3\n");
    }
    if ((PIN_pos_XY & (1 << pos_X4)) == 0) {
        xNu = 4;
        printf("xNu_4\n");
    }
    if ((PIN_pos_XY & (1 << pos_X5)) == 0) {
        xNu = 5;
        printf("xNu_5\n");
    }
}

void yNuFinder(void) {
    if ((PIN_pos_XY & (1 << pos_Y1)) == 0) {
        yNu = 1;
        printf("yNu_1\n");
    }
    if ((PIN_pos_XY & (1 << pos_Y2)) == 0) {
        yNu = 2;
        printf("yNu_2\n");
    }
    if ((PIN_pos_XY & (1 << pos_Y3)) == 0) {
        yNu = 3;
        printf("yNu_3\n");
    }
    if ((PIN_pos_Y  & (1 << pos_Y4)) == 0) {
        yNu = 4;
        printf("yNu_4\n");
    }
    if ((PIN_pos_Y  & (1 << pos_Y5)) == 0) {
        yNu = 5;
        printf("yNu_5\n");
    }
}

// Keypad uitlezen
char keypad_getkey(void) {
    static char lastKey = 0;
    char key = 0;

    for (uint8_t r = 0; r < ROWS; r++) {
        PORTD &= ~0x0F;               // alle rijen laag
        PORTD |= (1 << rowPins[r]);   // actieve rij hoog
        _delay_us(50);                 // stabilisatie

        for (uint8_t c = 0; c < COLS; c++) {
            if (!(PINC & (1 << colPins[c]))) {
                key = keys[r][c];
                break;
            }
        }
        if (key) break;
    }

    // Single log: wacht tot loslaten
    if (key && key != lastKey) {
        _delay_ms(50); // debounce
        lastKey = key;
        return key;
    } else if (!key) {
        lastKey = 0;
    }

    return 0;
}


void processKey(char key) {
    switch (inputState) {
        case IDLE:
            printf("NUMPAD_idle\n");
            if (key == 'A') {
                printf("A_in\n");
                inputState = PICKUP_X;
            } else if (key == 'B') {
                printf("B_in\n");
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
            printf("PICKUP_X_in\n");
            if (key >= '1' && key <= '5') {
                xEind = key - '0';
                inputState = PICKUP_Y;
            } else {
                infoEindPosOpgehaald = 0;
                inputState = IDLE;  // wrong key cancels
            }
            break;

        case PICKUP_Y:
            printf("PICKUP_Y_in\n");
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
            printf("DROPOFF_X_in\n");
            if (key >= '1' && key <= '5') {
                xEindDropOf = key - '0';
                inputState = DROPOFF_Y;
            } else {
                infoEindPosOpgehaald = 0;
                inputState = IDLE;
            }
            break;

        case DROPOFF_Y:
            printf("DROPOFF_Y_in\n");
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
    printf("pickUp_and_DropOff_pos_in\n");
    cli();
    while (1) {
        if((infoEindPosOpgehaald == 1) && (infoEindPosOpgehaald2 == 1)){
                printf("pickUp_and_DropOff_pos_out\n");
                sei();
                startSlot = 1;
                break;
            }

        char key = keypad_getkey();
        if (key) {
            processKey(key);
        }
    }
}
