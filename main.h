#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pthread.h>

enum komunikat {REQ, ACK, INSIDE, RELEASE, STOP};
enum stany {czekamNaTunel, ide, czekamNaRelease, czekamNaInside, czekamNaAck};
extern stany stanBogacza; // czekamNaTunel / ide
extern stany stanWatku; // czekamNaRelease / czekamNaInside / ide
enum kierunki {tam, zPowrotem, brak };

extern int id_proc;
extern int LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP;
extern int zegar;
extern int wybranyTunel;

struct packet_t{
    kierunki kierunek;
    int nr_tunelu;
    int rozmiar_grupy;
    int proc_zegar;
    int proc_id;
};
extern MPI_Datatype MPI_PAKIET_T;
extern int ID_WATKU_KOM;

void mySleep(int);

#define MPISOURCE = {1, 2, 3, 4, 5, 6};

#ifdef DEBUG
#define debug(FORMAT,...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(id_proc/7))%2, 31+(6+id_proc)%7, id_proc, ##__VA_ARGS__, 27,0,37);
#else
#define debug(...) ;
#endif

#endif
