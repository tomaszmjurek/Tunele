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
void zwiekszZegar();

extern int zapisanyZegar;
extern kierunki wybranyKierunek;
extern vector<int> kolejkaDoTunelu;

extern pthread_cond_t PRZEKAZ_RELEASE, PRZEKAZ_INSIDE, PRZEKAZ_ACK;
extern pthread_mutex_t mutex;

#endif