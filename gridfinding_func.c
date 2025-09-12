#include <avr/io.h>
#include "gridfinding_def.h"

// Vergelijkingen
int xNu_TOV_xEind(int nu, int eind) {
    if (nu > eind)  return 1;   // boven eind
    if (nu < eind)  return 0;   // onder eind
    return 2;                   // gelijk
}

int yNu_TOV_yEind(int nu, int eind) {
    if (nu > eind)  return 1;
    if (nu < eind)  return 0;
    return 2;
}

// Motorsturing
int motorX(int richting) {
    switch (richting) {
        case 0: // rechtsom
            while (xNu != xEind)portHBrug_X &= ~(1 << pinHBrug_RechtsOm_X);
            portHBrug_X |=  (1 << pinHBrug_RechtsOm_X);
            return 1;

        case 1: // linksom
            while (xNu != xEind) portHBrug_X &= ~(1 << pinHBrug_LinksOm_X);
            portHBrug_X |=  (1 << pinHBrug_LinksOm_X);
            return 1;

        case 2: // stil
        default:
            return 0;
    }
}

int motorY(int richting) {
    switch (richting) {
        case 0: // rechtsom
            while (yNu != yEind) portHBrug_Y &= ~(1 << pinHBrug_RechtsOm_Y);
            portHBrug_Y |=  (1 << pinHBrug_RechtsOm_Y);
            return 1;

        case 1: // linksom
            while (yNu != yEind) portHBrug_Y &= ~(1 << pinHBrug_LinksOm_Y);
            portHBrug_Y |=  (1 << pinHBrug_LinksOm_Y);
            return 1;

        case 2: // stil
        default:
            return 0;
    }
}

// Machine naar (1,1) sturen
void homeSender(void) {
    while (xNu != 1) portHBrug_X &= ~(1 << pinHBrug_LinksOm_X);
    portHBrug_X |=  (1 << pinHBrug_LinksOm_X);

    while (yNu != 1) portHBrug_Y &= ~(1 << pinHBrug_LinksOm_Y);
    portHBrug_Y |=  (1 << pinHBrug_LinksOm_Y);

    homeSenderDone = 1;
}



/*#include <avr/io.h>
#include <stdlib.h>
#include "gridfinding_def.h"

// als de motor linksom draait worden de waarde van x/y-Nu kleiner en rechtsom groter.

extern int xNu;
extern int xEind;
extern int yNu;
extern int yEind;
extern int homeSenderDone;

int xNu_TOV_xEind(int nu, int eind) {
    // Checked of de huidige x-positie boven of onder de doel-positie ligt
    if (nu > eind) {
        return 1;   // nu ligt boven eind
    }
    if (nu < eind) {
        return 0;   // nu ligt onder eind
    }
    if (nu == eind) {
        return 2;   // nu is gelijk aan eind
    }
    return -1; // failsafe (zou nooit moeten gebeuren)
}

int yNu_TOV_yEind(int nu, int eind) {
    // Checked of de huidige y-positie boven of onder de doel-positie ligt
    if (nu > eind) {
        return 1;   // nu ligt boven eind
    }
    if (nu < eind) {
        return 0;   // nu ligt onder eind
    }
    if (nu == eind) {
        return 2;   // nu is gelijk aan eind
    }
    return -1; // failsafe (zou nooit moeten gebeuren)
}

int motorX(int xPos) {
    switch(xPos){
        case 0:
            if(xNu != xEind){
                portHBrug_X &= ~(1 << pinHBrug_RechtsOm_X);//motorX doet iets
            }else{
                portHBrug_X |= (1 << pinHBrug_RechtsOm_X);
            }
            break;
        case 1:
            if(xNu != xEind){
                portHBrug_X &= ~(1 << pinHBrug_LinksOm_X);//motorX doet iets
            }else{
                portHBrug_X |= (1 << pinHBrug_LinksOm_X);
            }
            break;
        case 2:
            return 0;   //motorX doet niets
        default:
            exit(1);     //stop programma
    }
}

int motorY(int yPos) {
    switch(yPos){
        case 0:
            if(yNu != yEind){
                portHBrug_Y &= ~(1 << pinHBrug_RechtsOm_Y);//motorX doet iets
            }else{
                portHBrug_Y |= (1 << pinHBrug_RechtsOm_Y);
            }
            break;
        case 1:
            if(yNu != yEind){
                portHBrug_Y &= ~(1 << pinHBrug_LinksOm_Y);//motorX doet iets
            }else{
                portHBrug_Y |= (1 << pinHBrug_LinksOm_Y);
            }
            break;
        case 2:
            return 0;   //motorX doet niets
        default:
            exit(1);     //stop programma
    }
}

void homeSender(void)   //stuurt de machine naar (1,1)
{
    if(xNu != 1){
        portHBrug_X &= ~(1 << pinHBrug_LinksOm_X);
    }else{
        portHBrug_X |= (1 << pinHBrug_LinksOm_X);
    }

    if(yNu != 1){
        portHBrug_Y &= ~(1 << pinHBrug_LinksOm_Y);
    }else{
        portHBrug_Y |= (1 << pinHBrug_LinksOm_Y);
    }
    homeSenderDone = 1;
}*/
