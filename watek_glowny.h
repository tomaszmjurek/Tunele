#ifndef WATEK_GLOWNY_H
#define WATEK_GLOWNY_H

#include <algorithm>
#include <vector>
using namespace std;
/* pętla główna aplikacji: zmiany stanów */
void mainLoop();
void czekajNaWejscie(kierunki);
void przejdzTunelem(kierunki);
void krainaSzczesliwosci();
void dojdzDoSiebie();

void obsluzKolejkeDoTunelu(int);
bool obcyMaPierwszenstwo(packet_t);

extern int zapisanyZegar;//, wybranyTunel;
extern kierunki wybranyKierunek; // czy bedzie widziany deklaracja poza main.h
extern vector<int> kolejkaDoTunelu;

#endif