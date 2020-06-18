#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
#include <mpi.h>
#include <pthread.h>

//#include <pthread.h>

enum komunikat {REQ, ACK, INSIDE, RELEASE, STOP};
enum stany {czekam, zajety};
enum kierunki {tam, zPowrotem, brak };

extern int id_proc; //temp
extern int LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP;

// #ifdef DEBUG
// #define debug(FORMAT,...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(rank/7))%2, 31+(6+rank)%7, rank, ##__VA_ARGS__, 27,0,37);
// #else
// #define debug(...) ;
// #endif

#endif