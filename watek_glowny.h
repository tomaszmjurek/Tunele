#ifndef WATEK_GLOWNY_H
#define WATEK_GLOWNY_H

#include <algorithm>

/* pętla główna aplikacji: zmiany stanów */
void mainLoop();
void czekajNaWejscie(kierunki);
void przejdzTunelem(kierunki);
void krainaSzczesliwosci();
void dojdzDoSiebie();

void obsluzKolejkeDoTunelu(int);

extern volatile int zapisanyZegar, wybranyTunel;
extern volatile kierunki wybranyKierunek; // czy bedzie widziany

#endif