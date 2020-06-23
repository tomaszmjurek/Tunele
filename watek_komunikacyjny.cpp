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
        MPI_Recv(&pakiet, 40 , MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        int typKomunikatu = status.MPI_TAG; // sprawdzic czy dziala zamiana
        debug("[KOM] Otrzymalem komunikat typu %d", typKomunikatu);
        switch(typKomunikatu) {
            case REQ:
                debug("[KOM] Otrzymalem REQ...");
                if /* bogacz czeka do tunelu */ (stanBogacza == czekamNaTunel && wybranyTunel != -1) {
                    pakiet = przygotujPakiet(wybranyTunel, wybranyKierunek, zapisanyZegar);
                // TODO: nie powinniśmy zerować structa?
                    // Wyślij ACK z zegarem swojego REQ i numerem tunelu (jesli nie -1) o który się ubiegasz 
                    MPI_Send(&pakiet, 40, MPI_PAKIET_T, i, komunikat, MPI_COMM_WORLD);
                }
                oczekujace++;
                break;
            case INSIDE:
                debug("[KOM] Otrzymalem INSIDE...");
                dodajDoTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                oczekujace--;
                if (stanBogacza == czekamNaTunel && stanWatku == czekamNaInside) {
                   obsluzKolejkeDoTunelu(pakiet.proc_id);
                   MPI_SendLocal(PRZEKAZ_INSIDE);
                }
                break;
            case RELEASE:
                debug("[KOM] Otrzymalem RELEASE...");
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                //warunek z kolejką procesów w Tunelu
                if (stanWatku == czekamNaRelease) {
                    MPI_SendLocal(PRZEKAZ_RELEASE);
                }
                break;
            case STOP:
                debug("[KOM] Otrzymałem STOP");
                //zwolnij pamiec, zabij procesy
                dontStop = false;
                terminate();
            default:
                debug("[KOM] Otrzymalem błędny komunikat");
        }
    }
}

packet_t przygotujPakiet(int nr_tunelu, kierunki gdzie, int zapisanyZegar) {
    packet_t pakiet;
    pakiet.kierunek = gdzie;
    pakiet.nr_tunelu = nr_tunelu;
    pakiet.rozmiar_grupy = ROZMIAR_EKIPY;
    pakiet.proc_zegar = zapisanyZegar;
    pakiet.proc_id = id_proc;

    return pakiet;
}

void MPI_Broadcast(int nr_tunelu, kierunki gdzie, int zapisanyZegar, komunikat komunikat) {
    packet_t pakiet = przygotujPakiet(nr_tunelu, gdzie, zapisanyZegar);
    for (int i = 0; i < LICZBA_EKIP; i++) {
        if (i != id_proc) {
            MPI_Send(&pakiet, 40, MPI_PAKIET_T, i, komunikat, MPI_COMM_WORLD);
        }
    }
}

void MPI_SendLocal(komunikat komunikat) {
    MPI_Send(0, 10, MPI_PAKIET_T, ID_WATKU_KOM, komunikat, MPI_COMM_WORLD);
}

void MPI_RecvLocal(komunikat komunikat) {
    MPI_Status status;
    MPI_Recv(0, 10 , MPI_PAKIET_T, ID_WATKU_KOM, komunikat, MPI_COMM_WORLD, &status);
}
