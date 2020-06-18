#ifndef WATEK_GLOWNY_H
#define WATEK_GLOWNY_H
//#include <windows.h>
// #include "main.h"
// #include "tunel.cpp"

/* pętla główna aplikacji: zmiany stanów */
void mainLoop();
void czekajNaWejscie(kierunki);
void przejdzTunelem(kierunki);
void krainaSzczesliwosci();
void dojdzDoSiebie();

#endif