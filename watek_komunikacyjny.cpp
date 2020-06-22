#include "main.h"
#include <string>
using namespace std;
#include "watek_komunikacyjny.h"
#include "watek_glowny.h"
#include "tunel.h"

volatile int oczekujace;
bool dontStop = true;

packet_t pakiet;

int wysylajacy;

void *startWatekKom(void *ptr) {
    MPI_Status status;
    while (dontStop) {
        debug("Czekam na recv");
        MPI_Recv( &pakiet, 40 , MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        komunikat komunikat = REQ;//status.MPI_TAG;
        // wysylajacy = MPI_SOURCE;
        switch(komunikat) {
            case REQ:
                debug("Otrzymalem REQ...");
                if (stanBogacza == czekamNaTunel) {
                // TODO: nie powinniśmy zerować structa?
                    // Wyślij ACK z zegarem swojego REQ i numerem tunelu o który się ubiegasz 
                    // czyli zapisanyZegar musi byc wspoldzielony, wysylamy do wszystkich czy do jednego?
                }
                oczekujace++;
                break;
            case INSIDE:
                debug("Otrzymalem INSIDE...");
                if (stanBogacza == czekamNaTunel && stanWatku == czekamNaInside)
                   usunZKolejkiDoTuneluJesliIstnieje(pakiet.proc_id);
                oczekujace--;
                dodajDoTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                //usuniecie z kolejkiDoTunelu
                break;
            case RELEASE:
                debug("Otrzymalem RELEASE...");
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                //warunek z kolejką procesów w Tunelu
                if (stanWatku == czekamNaRelease) stanWatku = ide; // do petli while
                break;
            case STOP:
                //zwolnij pamiec, zabij procesy
                dontStop = false;
                terminate();
            default:
                debug("Otrzymalem błędny komunikat");
        }
    }
}

packet_t przygotujPakiet(int nr_tunelu, kierunki gdzie) {
    packet_t pakiet;
    pakiet.kierunek = gdzie;
    pakiet.nr_tunelu = nr_tunelu;
    pakiet.rozmiar_grupy = ROZMIAR_EKIPY;
    pakiet.proc_zegar = zegar;
    pakiet.proc_id = id_proc;

    return pakiet;
}

void MPI_Broadcast(int nr_tunelu, kierunki gdzie, komunikat komunikat) {
    packet_t pakiet = przygotujPakiet(nr_tunelu, gdzie);
    for (int i = 0; i < LICZBA_EKIP; i++) {
        if (i != id_proc) {
            MPI_Send(&pakiet, 40, MPI_PAKIET_T, i, komunikat, MPI_COMM_WORLD);
        }
    }
}