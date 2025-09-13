#include <avr/io.h>
#include <util/delay.h>
#include "gridfinding_def.h"
extern volatile int xEind, yEind;
extern volatile int xEindDropOf, yEindDropOf;

enum MagnetState {
    Get = 1,
    Drop = 2
};

void coordSwitcheroo(void){
    xEind = xEindDropOf;
    yEind = yEindDropOf;
}

int oppakProgrammaatje(enum MagnetState state){
    if(state == Get) portMagneet &= ~(1 << pinMagneet);
    if(state == Drop) portMagneet |= (1 << pinMagneet);
}

int motorZ(int opNeer) {  //links-/rechts-om zorgen bij de z-as voor en beweging om-hoog/-laag.
    if(opNeer == 1){
        while ((PIN_pos_Z & (1 << pos_Z)) != 1) portHBrug_Z &= ~(1 << pinHBrug_RechtsOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_RechtsOm_Z);

        oppakProgrammaatje(opNeer); //object vast

        while ((PIN_pos_Z & (1 << pos_Z)) != 1) portHBrug_Z &= ~(1 << pinHBrug_LinksOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_LinksOm_Z);

        coordSwitcheroo();

    }

    if(opNeer == 2){
        while ((PIN_pos_Z & (1 << pos_Z)) != 1) portHBrug_Z &= ~(1 << pinHBrug_LinksOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_LinksOm_Z);

        oppakProgrammaatje(opNeer); //object los

        while ((PIN_pos_Z & (1 << pos_Z)) != 1) portHBrug_Z &= ~(1 << pinHBrug_RechtsOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_RechtsOm_Z);

        opNeer = 1;

        //coordLock
    }
}
