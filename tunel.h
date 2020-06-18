#ifndef TUNEL_H
#define TUNEL_H
#include "main.h"

int znajdzMiejsceWTunelu(kierunki gdzie);
void dodajDoTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie);
void usunZTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie);
 
class Tunel {
    public:
        int zajetosc;
        kierunki kierunek;
};

Tunel * tunele;

#endif