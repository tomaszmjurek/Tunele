#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "tunel.h"

using namespace std;

int id_proc, ID_WATKU_KOM, LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP; /* zmienne statyczne globalne */
int zegar = 0;
stany stanBogacza = ide, stanWatku = ide;

pthread_t watekKom;
MPI_Datatype MPI_PAKIET_T;

void finalizuj()
{
    // pthread_mutex_destroy( &stateMut);
    /* Czekamy, aż wątek potomny się zakończy */
    // println("czekam na wątek \"komunikacyjny\"\n" );
    // pthread_join(threadKom,NULL);
    // if (rank==0) pthread_join(threadMon,NULL);
    //MPI_Type_free(&MPI_PAKIET_T);
    MPI_Finalize();
}

void inicjujMPI(int *argc, char *argv[]) {

    int provided;
    MPI_Init_thread(argc, &argv, MPI_THREAD_MULTIPLE, &provided); //3 w moim systemie?
    if (provided < MPI_THREAD_MULTIPLE) {
        debug("Error - MPI does not provide needed threading level");
        finalizuj();
        //return x
    }
    MPI_Comm_size(MPI_COMM_WORLD, &LICZBA_EKIP);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
    ID_WATKU_KOM = 200 + id_proc;
}


int main(int argc, char *argv[]) {

    inicjujMPI(&argc, argv);

    // Przygotowanie danych
    if (argc == 3) { // == 5
        LICZBA_TUNELI = atoi(argv[1]);
        POJEMNOSC_TUNELU = atoi(argv[2]); //wieksza niz min 1 ekipa
        LICZBA_EKIP = atoi(argv[3]);
    } else {
        LICZBA_TUNELI = 5;
        POJEMNOSC_TUNELU = 40;
    }

    if (POJEMNOSC_TUNELU < ROZMIAR_EKIPY) {
        debug("Pojemność tunelu za mała, zalecane > 40");
        finalizuj();
        return 1;
    }

    tunele = new Tunel [LICZBA_TUNELI];
    czyscTunele();

    ROZMIAR_EKIPY = rand() % 21 + 10; //rand od nowa

    debug("Bogacz gotowy! Tunele: %d, Pojemnosc: %d, Ekipa: %d", LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY);

    pthread_create(&watekKom, NULL, startWatekKom, 0);

    mainLoop(); // ew. osobny watek

    debug("Bogacz znika");
    finalizuj();

    return 0;
}

