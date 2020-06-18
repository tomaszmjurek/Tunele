#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include "tunel.h"

using namespace std;
int id_proc, LICZBA_TUNELI, POJEMNOSC_TUNELU, ROZMIAR_EKIPY, LICZBA_EKIP; /* zmienne statyczne globalne */
pthread_t watekKom, watekGlowny;

void inicjujMPI(int argc, char *argv[]) {

    MPI_Init(NULL, NULL);

}

int main(int argc, char *argv[]) {

    inicjujMPI(argc, argv);

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
        return 1;
    }

    tunele = new Tunel [LICZBA_TUNELI];
    czyscTunele();

    ROZMIAR_EKIPY = rand() % 21 + 10;

    id_proc = 0; //MPI get_id

    pthread_create(&watekKom, NULL, startWatekKom, 0);
    mainLoop(); // ew. osobny watek

    MPI_Finalize();

    return 0;
}

