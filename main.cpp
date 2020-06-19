#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "tunel.h"

using namespace std;

int id_proc, LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP; /* zmienne statyczne globalne */
pthread_t watekKom, watekGlowny;
MPI_Datatype MPI_PAKIET_T;

void finalizuj()
{
    // pthread_mutex_destroy( &stateMut);
    /* Czekamy, aż wątek potomny się zakończy */
    // println("czekam na wątek \"komunikacyjny\"\n" );
    // pthread_join(threadKom,NULL);
    // if (rank==0) pthread_join(threadMon,NULL);
    // MPI_Type_free(&MPI_PAKIET_T);
    debug("Finishing process\n");
    MPI_Finalize();
}

void inicjujMPI(int *argc, char ***argv, int liczbaProcesow) {
    int provided;
    MPI_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &provided); //3 w moim systemie?
    debug("provided %d", provided);
    if (provided < MPI_THREAD_MULTIPLE) {
        debug("Error - MPI does not provide needed threading level");
        finalizuj();
    }
    MPI_Comm_size(MPI_COMM_WORLD, &liczbaProcesow);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
    debug("Started process");
}


int main(int argc, char **argv) {

    LICZBA_EKIP = 3; //hardcoded
    inicjujMPI(&argc, &argv, LICZBA_EKIP);

    // Przygotowanie danych
    if (argc > 0) { // == 5
        LICZBA_TUNELI = atoi(argv[1]);
        POJEMNOSC_TUNELU = atoi(argv[2]); //wieksza niz min 1 ekipa
        LICZBA_EKIP = atoi(argv[3]);
    } else {
        LICZBA_TUNELI = 5;
        POJEMNOSC_TUNELU = 40;
        LICZBA_EKIP = 15;
    }

    if (POJEMNOSC_TUNELU < ROZMIAR_EKIPY) {
        cout << "Pojemność tunelu za mała, zalecane > 40" << endl;
        finalizuj();
        return 1;
    }

    tunele = new Tunel [LICZBA_TUNELI];
    czyscTunele();

    ROZMIAR_EKIPY = rand() % 21 + 10;

    // pthread_create(&watekKom, NULL, startWatekKom, 0);
    // mainLoop(); // ew. osobny watek
    //cout << "Running MPI proces rank: " << id_proc << endl;
    debug("Running MPI");

    finalizuj();

    return 0;
}

