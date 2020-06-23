#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <pthread.h>

enum komunikat {REQ, ACK, INSIDE, RELEASE, STOP, PRZEKAZ_INSIDE, PRZEKAZ_RELEASE};
enum stany {czekamNaTunel, ide, czekamNaRelease, czekamNaInside};
extern volatile stany stanBogacza; // czekamNaTunel / ide
extern volatile stany stanWatku; // czekamNaRelease / czekamNaInside / ide
enum kierunki {tam, zPowrotem, brak };

extern int id_proc;
extern int LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP;
extern volatile int zegar;
extern volatile int oczekujace;

typedef struct {
    kierunki kierunek;
    int nr_tunelu;
    int rozmiar_grupy;
    int proc_zegar;
    int proc_id;
}packet_t;
extern MPI_Datatype MPI_PAKIET_T;
extern int ID_WATKU_KOM;

#ifdef DEBUG
#define debug(FORMAT,...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(id_proc/7))%2, 31+(6+id_proc)%7, id_proc, ##__VA_ARGS__, 27,0,37);
#else
#define debug(...) ;
#endif

#endif
