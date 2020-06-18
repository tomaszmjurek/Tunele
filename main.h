#ifndef GLOBALH
#define GLOBALH

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

//#include <pthread.h>

typedef enum komunikaty {REQ, ACK, INSIDE, RELEASE, STOP};
typedef enum stany {czekam, zajety};
typedef enum kierunki {tam, zPowrotem, brak };
struct package{
    int kierunek;
    int nr_tunelu;
    int rozmiar_grupy;
    int proc_zegar;
    int proc_id;
}packet_t
extern MPI_Datatype MPI_PAKIET_T;

int id_proc; //temp
int LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP;

#ifdef DEBUG
#define debug(FORMAT,...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(rank/7))%2, 31+(6+rank)%7, rank, ##__VA_ARGS__, 27,0,37);
#else
#define debug(...) ;
#endif

#endif