#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "tunel.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include <time.h>
#include <mpi.h>
// #include <typeinfo>
using namespace std;

int id_proc, ID_WATKU_KOM, LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP; /* zmienne statyczne globalne */
int zegar = 0, wybranyTunel = 0;
stany stanBogacza = ide, stanWatku = ide;

pthread_t watekKom;
MPI_Datatype MPI_PAKIET_T; // todo: raczej zbedne

void finalizuj() // testowalem wszystkie te mozliwosci najlepiej dziala dontStop, jest error ale zatzymuje sie sam
{
  //  pthread_detach(watekKom); 
//     ~watekKom;
    // pthread_exit(watekKom);
  //  MPI_Broadcast(-1, brak, zegar, STOP);
    // pthread_join(watekKom, NULL);
    
    dontStop = false; // niedoskonałe, bo musi otrzymac Recv, w praktyce zawsze kiedys dostanie, pytanie jak danilecki chce zeby sie konczylo, po ilus iteracjach czy ctrl + c
    debug("[MAIN] Bogacz [%d] zaraz zniknie", id_proc); // todo: fix error
    MPI_Barrier(MPI_COMM_WORLD);
    // if (id_proc == 0) wait
    MPI_Finalize();
}

void mySleep(int time) {
    int j = 0;
    double x = 0.0, y = 0.1;
    while (j < time * 10000)
    {
        x *= x / y;
        j++;
    }
}

void inicjujMPI(int *argc, char *argv[]) {
    int provided;
    MPI_Init_thread(argc, &argv, MPI_THREAD_MULTIPLE, &provided); //3 w moim systemie?
    if (provided < MPI_THREAD_MULTIPLE) {
        debug("Error - MPI does not provide needed threading level");
        finalizuj();
    }
    MPI_Comm_size(MPI_COMM_WORLD, &LICZBA_EKIP);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
    ID_WATKU_KOM = 200 + id_proc;
}


int main(int argc, char *argv[]) {
    sleep(1000);
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

    debug("Bogacz [%d] gotowy! Tunele: %d, Pojemnosc po: %d, Ekipy: %d,  Moja ekipa: %d", id_proc, LICZBA_TUNELI, POJEMNOSC_TUNELU, LICZBA_EKIP, ROZMIAR_EKIPY);

    if (POJEMNOSC_TUNELU < ROZMIAR_EKIPY) {
        debug("Pojemność tunelu za mała, zalecane 30-70");
        finalizuj();
        return 1;
    }

    tunele = new Tunel [LICZBA_TUNELI];
    czyscTunele();

    pthread_create(&watekKom, NULL, startWatekKom, 0);

    MPI_Barrier(MPI_COMM_WORLD);
    mainLoop();

    finalizuj();
    debug("Bogacz znika");

    return 0;
}

