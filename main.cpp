#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "tunel.h"
#include <time.h>
// #include <stdio.h>
#include <unistd.h>
// #include <time.h>
// #include <mpi.h>
using namespace std;

int id_proc, ID_WATKU_KOM, LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP; /* zmienne statyczne globalne */
int zegar = 0, wybranyTunel = 0;
stany stanBogacza = ide, stanWatku = ide;

pthread_t watekKom;

void finalizuj() {
    dontStop = false;
    debug("[MAIN] Bogacz [%d] zaraz zniknie", id_proc);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

void inicjujMPI(int *argc, char *argv[]) {
    int provided;
    MPI_Init_thread(argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided < MPI_THREAD_MULTIPLE) {
        debug("Error - MPI does not provide needed threading level");
        finalizuj();
    }
    MPI_Comm_size(MPI_COMM_WORLD, &LICZBA_EKIP);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
}


int main(int argc, char *argv[]) {
    inicjujMPI(&argc, argv);

    // Przygotowanie danych
    if (argc == 2) { 
        LICZBA_TUNELI = atoi(argv[1]);
        POJEMNOSC_TUNELU = 40;
    } else if (argc == 3){
        LICZBA_TUNELI = atoi(argv[1]);
        POJEMNOSC_TUNELU = atoi(argv[2]);
    } else {
        LICZBA_TUNELI = 2;
        POJEMNOSC_TUNELU = 45;
    }

    srand(time(NULL) + id_proc);
    ROZMIAR_EKIPY = rand() % 21 + 10;

    debug("Bogacz [%d] gotowy! Tunele: %d, Pojemnosc po: %d, Ekipy: %d,  Moja ekipa: %d", 
                    id_proc, LICZBA_TUNELI, POJEMNOSC_TUNELU, LICZBA_EKIP, ROZMIAR_EKIPY);

    if (POJEMNOSC_TUNELU < ROZMIAR_EKIPY) {
        debug("Pojemność tunelu za mała, zalecane 30-70");
        finalizuj();
        return 1;
    }

    tunele = new Tunel [LICZBA_TUNELI];
    czyscTunele();

    pthread_create(&watekKom, NULL, startWatekKom, 0);

    MPI_Barrier(MPI_COMM_WORLD); // równy start procesów

    mainLoop();

    finalizuj();
    debug("Bogacz znika");

    return 0;
}

