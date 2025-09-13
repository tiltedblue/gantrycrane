#include <avr/io.h>
#include <util/delay.h>
#include "gridfinding_def.h"

void coordSwitcheroo(void){
    xEind = xEindDropOf;
    yEind = yEindDropOf;
    heenTerug = 2;
}

int oppakProgrammaatje(enum MagnetState state){
    if(state == Get) portMagneet &= ~(1 << pinMagneet);
    if(state == Drop) portMagneet |= (1 << pinMagneet);

    return 0;
}

void eindProgrammaatje(void){
    heenTerug = 1;
    portMagneet |= (1 << pinMagneet);
    infoEindPosOpgehaald = 0;
    infoEindPosOpgehaald2 = 0;
    homeSender();
    startSlot = 0;
    homeSenderDone = 0;
    startKnop = 0;
}

int motorZ(int opNeer) {  //links-/rechts-om zorgen bij de z-as voor en beweging om-hoog/-laag.
    if(opNeer == 1){
        while ((PIN_pos_Z & (1 << pos_Z)) == 0) portHBrug_Z &= ~(1 << pinHBrug_RechtsOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_RechtsOm_Z);

        oppakProgrammaatje(opNeer); //object vast

        while ((PIN_pos_Z & (1 << pos_Z)) == 0) portHBrug_Z &= ~(1 << pinHBrug_LinksOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_LinksOm_Z);

        coordSwitcheroo();
    }

    if(opNeer == 2){
        while ((PIN_pos_Z & (1 << pos_Z)) == 0) portHBrug_Z &= ~(1 << pinHBrug_LinksOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_LinksOm_Z);

        oppakProgrammaatje(opNeer); //object los

        while ((PIN_pos_Z & (1 << pos_Z)) == 0) portHBrug_Z &= ~(1 << pinHBrug_RechtsOm_Z);
        portHBrug_Z |=  (1 << pinHBrug_RechtsOm_Z);

        eindProgrammaatje();


    }
    return 0;
}
