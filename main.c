#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "gridfinding_def.h"

// === Definitie globale variabelen ===
volatile int startKnop = 0;
volatile int homeSenderDone = 0;
volatile int startSlot = 0;

volatile int xNu = 0, yNu = 0;
volatile int xEind = 0, yEind = 0;

volatile int infoEindPosOpgehaald = 0;
volatile int info_X_Opgehaald = 1;

// === Timer1 init ===
void init_timer1(void) {
    TCCR1A = 0;
    TCCR1B = 0b00000101; // prescaler 1024
    TIMSK1 = 0b00000001; // overflow interrupt enable
    sei();
    TCNT1 = 63973;       // startwaarde voor ~10 ms
}

// === Keypad init ===
void keypad_init(void) {
    ROW_DDR = 0x0F;   // rijen als output
    ROW_PORT = 0x00;
    COL_DDR &= ~(0x0F); // kolommen als input
    COL_PORT |= 0x0F;   // pull-ups aan
}

// === Timer ISR ===
ISR(TIMER1_OVF_vect) {
    xNuFinder();
    yNuFinder();
    TCNT1 = 63973; // reset timer
}

// === Main loop ===
int main(void) {
    init_timer1();
    keypad_init();

    while (1) {
        if ((PIN_pos_XY & (1 << pos_X1)) || (startKnop == 1)) {
            startKnop = 1;

            if (homeSenderDone == 0)
                homeSender();

            if ((xNu == xEind) && (yNu == yEind))
                info_X_Opgehaald = 0;

            if ((infoEindPosOpgehaald == 1) && (startSlot == 1)) {
                motorX(xNu_TOV_xEind(xNu, xEind));
                motorY(yNu_TOV_yEind(yNu, yEind));
            }

            if ((xNu == xEind) && (yNu == yEind) && (startSlot == 1)) {
                motorZ();
                infoEindPosOpgehaald = 0;
            }
        }
    }
    return 0;
}





/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include "gridfinding_def.h"

//vergeet geen DDR aan te roepen

int startKnop = 0;  //de machine start pas op als deze knop is ingedrukt.
int homeSenderDone = 0;
int startSlot = 0;  //zorgt dat de eerste invoer kan worden gedaan.
int xNu,yNu;    //wat is de x/y-positie nu?
int xEind = 0, yEind = 0;   //waar moet de kraan heen?
int infoEindPosOpgehaald = 0;   //Zorgt ervoor dat de motoren niet kunnen draaien tot er een invoer is gegeven voor x/y-Eind.
int info_X_Opgehaald = 1;   //zorgt ervoor dat de x-positie eerder wordt ingevoerd dan de y-positie.



void init_timer1()
{
    TCCR1A = 0b00000000;
    TCCR1B = 0b00000101;
    TCCR1C = 0b00000000;
    TIMSK1 = 0b00000001;
    sei();
}

void keypad_init() {
    ROW_DDR = 0x0F;    // rijen als output
    ROW_PORT = 0x00;

    COL_DDR &= ~(0x0F); // kolommen als input
    COL_PORT |= 0x0F;   // pull-up
}

ISR(TIMER1_OVF_vect) //checked de positie van de kraan elke 10ms.
{
    xNuFinder();
    yNuFinder();
    TCNT1 = 63973;
}


int main(void)
{
    init_timer1();
    keypad_init();

    while(1)
    {

        if((PIN_pos_XY & (1 << pos_X1)) || (startKnop == 1))
        {
            startKnop = 1;
            if(homeSenderDone == 0)
                homeSender();

            if((xNu == xEind) && (yNu == yEind))
                info_X_Opgehaald = 0;

            //pickUp_and_DropOff_pos();

            if((infoEindPosOpgehaald == 1) && (startSlot == 1))
            {
                motorX(xNu_TOV_xEind(xNu,xEind));

                motorY(yNu_TOV_yEind(yNu,yEind));
            }

            if((xNu == xEind) && (yNu == yEind) && (startSlot == 1))
            {
                //start programma z-as

                infoEindPosOpgehaald = 0;
            }
        }


    }

    return 0;
}
 */



//to do: z-as programmeren

