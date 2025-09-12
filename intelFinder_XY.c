#include <avr/io.h>
#include <util/delay.h>
#include "gridfinding_def.h"

// Extra eindposities
int xEindDropOf = 0, yEindDropOf = 0;

// Statusflags
static int info_X_Opgehaald2 = 1;
static int infoEindPosOpgehaald2 = 0;

// Keypad mapping
static const char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

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
    return '0';
}

// Pickup positie
void xEindFinder1(char key) {
    if (((info_X_Opgehaald == 1) && (infoEindPosOpgehaald == 0)) || (startSlot == 0)) {
        if (key >= '1' && key <= '5') xEind = key - '0';

        info_X_Opgehaald = 2;
    }
}

void yEindFinder1(char key) {
    if (info_X_Opgehaald == 2) {
        if (key >= '1' && key <= '5') yEind = key - '0';

        info_X_Opgehaald = 0;
        infoEindPosOpgehaald = 1;
        startSlot = 1;
    }
}

// Dropoff positie
void xEindFinder2(char key) {
    if (((info_X_Opgehaald2 == 1) && (infoEindPosOpgehaald2 == 0)) || (startSlot == 0)) {
        if (key >= '1' && key <= '5') xEindDropOf = key - '0';

        info_X_Opgehaald2 = 2;
    }
}

void yEindFinder2(char key) {
    if (info_X_Opgehaald2 == 2) {
        if (key >= '1' && key <= '5') yEindDropOf = key - '0';

        info_X_Opgehaald2 = 0;
        infoEindPosOpgehaald2 = 1;
        startSlot = 1;
    }
}

// Pickup & dropoff workflow
void pickUp_and_DropOff_pos(void) {
    while (1) {
        char key = keypad_getkey();
        if (!key) continue;

        if (key == 'D') { // stop

            break;
        }

        if (key == 'A') {
            char xKey = keypad_getkey();
            if (xKey) xEindFinder1(xKey);
            char yKey = keypad_getkey();
            if (yKey) yEindFinder1(yKey);
        }

        if (key == 'B') {
            char xKey = keypad_getkey();
            if (xKey) xEindFinder2(xKey);
            char yKey = keypad_getkey();
            if (yKey) yEindFinder2(yKey);
        }
    }
}




/*#include <avr/io.h>
#include <util/delay.h>
#include "gridfinding_def.h"

extern int info_X_Opgehaald;
extern int infoEindPosOpgehaald;
extern int startSlot;

extern int xNu;
extern int xEind;
extern int yNu;
extern int yEind;

int xEindDropOf = 0, yEindDropOf = 0;

int info_X_Opgehaald2 = 1;
int infoEindPosOpgehaald2 = 0;

int pressToggleA = 0;
int pressToggleB = 0;

char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void xNuFinder(void)
{
    if(PIN_pos_XY & (1 << pos_X1))
        xNu = 1;
    if(PIN_pos_XY & (1 << pos_X2))
        xNu = 2;
    if(PIN_pos_XY & (1 << pos_X3))
        xNu = 3;
    if(PIN_pos_XY & (1 << pos_X4))
        xNu = 4;
    if(PIN_pos_XY & (1 << pos_X5))
        xNu = 5;
}

void yNuFinder(void)
{
    if(PIN_pos_XY & (1 << pos_Y1))
        yNu = 1;
    if(PIN_pos_XY & (1 << pos_Y2))
        yNu = 2;
    if(PIN_pos_XY & (1 << pos_Y3))
        yNu = 3;
    if(PIN_pos_Y & (1 << pos_Y4))
        yNu = 4;
    if(PIN_pos_Y & (1 << pos_Y5))
        yNu = 5;
}

// Keypad uitlezen
char keypad_getkey() {
    for (int r = 0; r < ROWS; r++) {
        ROW_PORT = (1 << r);
        _delay_us(5);

        for (int c = 0; c < COLS; c++) {
            if (!(COL_PIN & (1 << c))) {
                while (!(COL_PIN & (1 << c))); // wachten tot losgelaten
                _delay_ms(50); // debounce
                return keys[r][c];
            }
        }
    }
    return 0;
}

// Functies x/y coördinaten
void xEindFinder1(char key) {
    if (((info_X_Opgehaald == 1) && (infoEindPosOpgehaald == 0)) || (startSlot == 0)) {
        if (key >= '1' && key <= '5') xEind = key - '0';
        info_X_Opgehaald = 2;
    }
}

void yEindFinder1(char key) {
    if (info_X_Opgehaald == 2) {
        if (key >= '1' && key <= '5') yEind = key - '0';
        info_X_Opgehaald = 0;
        infoEindPosOpgehaald = 1;
        startSlot = 1;
    }
}

void xEindFinder2(char key) {
    if (((info_X_Opgehaald2 == 1) && (infoEindPosOpgehaald2 == 0)) || (startSlot == 0)) {
        if (key >= '1' && key <= '5') xEindDropOf = key - '0';
        info_X_Opgehaald2 = 2;
    }
}

void yEindFinder2(char key) {
    if (info_X_Opgehaald2 == 2) {
        if (key >= '1' && key <= '5') yEindDropOf = key - '0';
        info_X_Opgehaald2 = 0;
        infoEindPosOpgehaald2 = 1;
        startSlot = 1;
    }
}

// Pickup & Dropoff met continue aanpassen tot 'D'
void pickUp_and_DropOff_pos(void) {
    while (1) {
        char key = keypad_getkey();
        if (key) {
            if (key == 'D') {
                // Stop de functie zodra D is ingedrukt
                pressToggleA = 0;
                pressToggleB = 0;
                break;
            }

            if (key == 'A' || pressToggleA == 1) {
                pressToggleA = 1;
                char xKey = keypad_getkey();
                if (xKey) xEindFinder1(xKey);
                char yKey = keypad_getkey();
                if (yKey) yEindFinder1(yKey);
            }

            if (key == 'B' || pressToggleB == 1) {
                pressToggleB = 1;
                char xKey = keypad_getkey();
                if (xKey) xEindFinder2(xKey);
                char yKey = keypad_getkey();
                if (yKey) yEindFinder2(yKey);
            }
        }
    }
}*/




/*void pickUp_and_DropOff_pos(void)
{
    if( || (int pressToggleA == 1))   //knop A op numpad wordt ingedrukt
    {
        pressToggleA = 1;
        xEindFinder1();
        yEindFinder1();
    }
    if( || (int pressToggleB == 1))   //knop B op numpad wordt ingedrukt
    {
        pressToggleB = 1;
        xEindFinder2();
        yEindFinder2();
    }
}

void xEindFinder1(void) //voor pickup
{
    if(((info_X_Opgehaald == 1) && (infoEindPosOpgehaald == 0)) || (startSlot == 0))
    {
        if();   //knop 1 op numpad wordt ingedrukt
            xEind = 1;
        if();   //knop 2 op numpad wordt ingedrukt
            xEind = 2;
        if();   //knop 3 op numpad wordt ingedrukt
            xEind = 3;
        if();   //knop 4 op numpad wordt ingedrukt
            xEind = 4;
        if();   //knop 5 op numpad wordt ingedrukt
            xEind = 5;

        info_X_Opgehaald = 2;
    }
}

void yEindFinder1(void) //voor pickup
{
    if(info_X_Opgehaald == 2)
    {
        if();   //knop 1 op numpad wordt ingedrukt
            yEind = 1;
        if();   //knop 2 op numpad wordt ingedrukt
            yEind = 2;
        if();   //knop 3 op numpad wordt ingedrukt
            yEind = 3;
        if();   //knop 4 op numpad wordt ingedrukt
            yEind = 4;
        if();   //knop 5 op numpad wordt ingedrukt
            yEind = 5;

        info_X_Opgehaald = 0;
        infoEindPosOpgehaald = 1;
        startSlot = 1;
    }
}

void xEindFinder2(void) //voor dropoff
{
    if(((info_X_Opgehaald == 1) && (infoEindPosOpgehaald == 0)) || (startSlot == 0))
    {
        if();   //knop 1 op numpad wordt ingedrukt
            xEindDropOf = 1;
        if();   //knop 2 op numpad wordt ingedrukt
            xEindDropOf = 2;
        if();   //knop 3 op numpad wordt ingedrukt
            xEindDropOf = 3;
        if();   //knop 4 op numpad wordt ingedrukt
            xEindDropOf = 4;
        if();   //knop 5 op numpad wordt ingedrukt
            xEindDropOf = 5;

        info_X_Opgehaald2 = 2;
    }
}

void yEindFinder2(void) //voor dropoff
{
    if(((info_X_Opgehaald == 1) && (infoEindPosOpgehaald == 0)) || (startSlot == 0))
    {
        if();   //knop 1 op numpad wordt ingedrukt
            yEindDropOf = 1;
        if();   //knop 2 op numpad wordt ingedrukt
            yEindDropOf = 2;
        if();   //knop 3 op numpad wordt ingedrukt
            yEindDropOf = 3;
        if();   //knop 4 op numpad wordt ingedrukt
            yEindDropOf = 4;
        if();   //knop 5 op numpad wordt ingedrukt
            yEindDropOf = 5;

        info_X_Opgehaald2 = 0;
        infoEindPosOpgehaald2 = 1;
        startSlot = 1;
    }
}*/
