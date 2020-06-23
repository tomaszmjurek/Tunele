#include "main.h"
#include <string>
using namespace std;
#include "watek_komunikacyjny.h"
#include "watek_glowny.h"
#include "tunel.h"

int oczekujace = 0;
bool dontStop = true;

packet_t pakiet; //czy nie zerowac regularnie

void *startWatekKom(void *ptr) {
    MPI_Status status;
    while (dontStop) {
        MPI_Recv(&pakiet, sizeof(packet_t), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      //  int typKomunikatu = status.MPI_TAG; // sprawdzic czy dziala zamiana
        switch(status.MPI_TAG) {
            case REQ:
                debug("[KOM] Otrzymalem REQ od [%d]", pakiet.proc_id);
                if /* bogacz czeka do tunelu */ (stanBogacza == czekamNaTunel && wybranyTunel != -1) {
                    packet_t pakietWysylany = przygotujPakiet(wybranyTunel, wybranyKierunek, zapisanyZegar);
                    MPI_Send(&pakietWysylany, sizeof(packet_t), MPI_BYTE, pakiet.proc_id, ACK, MPI_COMM_WORLD);
                    debug("[KOM] Wyslalem ACK tunel: %d kierunek: %d zegar: %d", wybranyTunel, wybranyKierunek, zapisanyZegar)
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
                   debug("[KOM] Przekazalem INSIDE do watku_glownego");
                }
                break;
            case RELEASE:
                debug("[KOM] Otrzymalem RELEASE...");
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                //warunek z kolejką procesów w Tunelu
                if (stanWatku == czekamNaRelease) {
                    MPI_SendLocal(PRZEKAZ_RELEASE);
                    debug("[KOM] Przekazalem RELEASE do watku_glownego");
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
    packet_t pakiet__;
    pakiet__.kierunek = gdzie;
    pakiet__.nr_tunelu = nr_tunelu;
    pakiet__.rozmiar_grupy = ROZMIAR_EKIPY;
    pakiet__.proc_zegar = zapisanyZegar;
    pakiet__.proc_id = id_proc;

    return pakiet__;
}

void MPI_Broadcast(int nr_tunelu, kierunki gdzie, int zapisanyZegar, komunikat komunikat) {
    debug("MPIBroadcast");
    packet_t pakiet_ = przygotujPakiet(nr_tunelu, gdzie, zapisanyZegar);
    for (int i = 0; i < LICZBA_EKIP; i++) {
        if (i != id_proc) {
            debug("Wysylam %d do %d", komunikat, i);
            MPI_Send(&pakiet_, sizeof(packet_t), MPI_BYTE, i, komunikat, MPI_COMM_WORLD);
            debug("MPIBroadcast SEND to process %d", i);
        }
    }
}

void MPI_SendLocal(komunikat komunikat) {
    MPI_Send(0, sizeof(int), MPI_INT, ID_WATKU_KOM, komunikat, MPI_COMM_WORLD);
}

void MPI_RecvLocal(komunikat komunikat) {
    MPI_Status status;
    MPI_Recv(0, sizeof(int), MPI_INT, ID_WATKU_KOM, komunikat, MPI_COMM_WORLD, &status);
}
