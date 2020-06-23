#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "tunel.h"

using namespace std;

int id_proc, ID_WATKU_KOM, LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP; /* zmienne statyczne globalne */
int zegar = 0;
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
    debug("[MAIN] Bogacz zaraz zniknie"); // todo: fix error
    MPI_Finalize();
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

    debug("Bogacz gotowy! Tunele: %d, Ekipy: %d, Pojemnosc: %d, Ekipa: %d", LICZBA_TUNELI, LICZBA_EKIP, POJEMNOSC_TUNELU, ROZMIAR_EKIPY);

    pthread_create(&watekKom, NULL, startWatekKom, 0);

    mainLoop(); // ew. osobny watek

    finalizuj();
    debug("Bogacz znika");

    return 0;
}

