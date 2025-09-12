#ifndef GRIDFINDING_DEF_H_INCLUDED
#define GRIDFINDING_DEF_H_INCLUDED

#include <avr/io.h>

// === Globale variabelen ===
extern volatile int xNu;
extern volatile int yNu;
extern volatile int xEind;
extern volatile int yEind;

extern volatile int info_X_Opgehaald;
extern volatile int infoEindPosOpgehaald;
extern volatile int homeSenderDone;
extern volatile int startSlot;

// === Pinnen ===
#define pinStartKnop PF6
#define portStartKnop PORTF

// switches X-pos
#define pos_X1 PB0
#define pos_X2 PB1
#define pos_X3 PB2
#define pos_X4 PB3
#define pos_X5 PB4

// switches Y-pos
#define pos_Y1 PB5
#define pos_Y2 PB6
#define pos_Y3 PB7
#define pos_Y4 PF0
#define pos_Y5 PF1

// H-brug pinnen
#define pinHBrug_RechtsOm_X PF2
#define pinHBrug_RechtsOm_Y PF3
#define pinHBrug_LinksOm_X  PF4
#define pinHBrug_LinksOm_Y  PF5

#define portHBrug_X PORTF
#define portHBrug_Y PORTF

// pin check
#define PIN_pos_XY PINB
#define PIN_pos_Y  PINF

// Keypad
#define ROWS 4
#define COLS 4

#define ROW_PORT PORTD
#define ROW_PIN  PIND
#define ROW_DDR  DDRD

#define COL_PORT PORTC
#define COL_PIN  PINC
#define COL_DDR  DDRC

// === Functieprototypes ===
void xNuFinder(void);
void yNuFinder(void);
void homeSender(void);
void pickUp_and_DropOff_pos(void);
char keypad_getkey(void);

int xNu_TOV_xEind(int nu, int eind);
int yNu_TOV_yEind(int nu, int eind);
int motorX(int xPos);
int motorY(int yPos);

#endif
