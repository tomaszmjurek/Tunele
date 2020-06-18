#ifndef TUNEL_H
#define TUNEL_H
// #include "main.h"

//najwyzej zdef kierunki

int znajdzMiejsceWTunelu(kierunki gdzie);
void dodajDoTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie);
void usunZTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie);
void czyscTunele();
 
class Tunel {
    public:
        int zajetosc;
        kierunki kierunek;
};

extern Tunel * tunele;

#endif