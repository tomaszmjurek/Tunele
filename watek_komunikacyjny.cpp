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
    debug("[KOM] Odbieranie komunikatów w gotowości");

    MPI_Status status;
    while (dontStop) {
        MPI_Recv(&pakiet, sizeof(packet_t), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        switch(status.MPI_TAG) {
            case REQ:
                debug("[KOM] Otrzymalem REQ od [%d] tunel %d", pakiet.proc_id, pakiet.nr_tunelu);
                // debug("Dodaje do kolejki na probe");
                // kolejkaDoTunelu.push_back(pakiet.proc_id);
                oczekujace++; 
                if /* bogacz czeka do tunelu */ (stanBogacza == czekamNaTunel && wybranyTunel != -1) {
                    packet_t pakietWysylany = przygotujPakiet(wybranyTunel, wybranyKierunek, zapisanyZegar);
                    debug("Wysylam ACK do %d", pakiet.proc_id);
                    MPI_Send(&pakietWysylany, sizeof(packet_t), MPI_BYTE, pakiet.proc_id, ACK, MPI_COMM_WORLD);
                    debug("[KOM] Wyslalem ACK do [%d] tunel: %d kierunek: %d zegar: %d", pakiet.proc_id, wybranyTunel, wybranyKierunek, zapisanyZegar);
                }         
                break;
            case INSIDE:
                debug("[KOM] Otrzymalem INSIDE od [%d] tunel %d", pakiet.proc_id, pakiet.nr_tunelu);
                dodajDoTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                oczekujace--;
                tunele[pakiet.nr_tunelu].kolejkaWTunelu.push_back(pakiet.proc_id);
                // todo: prawdopodobnie stanyWatku beda zbedne dzieki cond_broadcast
                if (stanBogacza == czekamNaTunel && stanWatku == czekamNaInside) {
                   debug("[KOM] Przed INSIDE Kolejka do tunelu ma rozmiar: %ld", kolejkaDoTunelu.size()); 
                   obsluzKolejkeDoTunelu(pakiet.proc_id);
                   debug("[KOM] Po INSIDE Kolejka do tunelu ma rozmiar: %ld", kolejkaDoTunelu.size());
                //   MPI_SendLocal(PRZEKAZ_INSIDE);
                   pthread_cond_broadcast(&PRZEKAZ_INSIDE);
                   debug("[KOM] Przekazalem INSIDE do watku_glownego");
                }
                break;
            case RELEASE:
                debug("[KOM] Otrzymalem RELEASE od [%d] tunel %d", pakiet.proc_id, pakiet.nr_tunelu);
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy, pakiet.kierunek);
                kolejkaWTuneluPopFront(pakiet.nr_tunelu, pakiet.proc_id);
                if (stanWatku == czekamNaRelease) {
                //    MPI_SendLocal(PRZEKAZ_RELEASE);
                    pthread_cond_broadcast(&PRZEKAZ_RELEASE);
                    debug("[KOM] Przekazalem RELEASE do watku_glownego");
                }
                break;
            case ACK:
                debug("[KOM] Otrzymalem ACK od [%d]", pakiet.proc_id);
                if (stanWatku == czekamNaAck) {
                    if (pakiet.nr_tunelu == wybranyTunel && obcyMaPierwszenstwo(pakiet)) {
                        kolejkaDoTunelu.push_back(pakiet.proc_id); // rezygnuje z zapisaywanie proc_zegar
                        debug("[TEST] No co Pan sie wpycha");
                    }
                //    MPI_SendLocal(PRZEKAZ_ACK);
                    pthread_cond_broadcast(&PRZEKAZ_ACK);
                }
                break;
            case STOP:
                debug("Otrzymałem stop od id_proc %d", pakiet.proc_id);
                debug("[KOM] Otrzymałem STOP. Aktualny zegar: %d", zegar);
                //zwolnij pamiec, zabij procesy
                dontStop = false;
                terminate(); //todo remove?
                break;
            default:
                debug("[KOM] Otrzymalem błędny komunikat o tagu %d", status.MPI_TAG);
        }
    }
    return 0;
}

packet_t przygotujPakiet(int nr_tunelu, kierunki gdzie, int zapisanyZegar) {
    packet_t pakiet_;
    pakiet_.kierunek = gdzie;
    pakiet_.nr_tunelu = nr_tunelu;
    pakiet_.rozmiar_grupy = ROZMIAR_EKIPY;
    pakiet_.proc_zegar = zapisanyZegar;
    pakiet_.proc_id = id_proc;

    return pakiet_;
}

void MPI_Broadcast(int nr_tunelu, kierunki gdzie, int zapisanyZegar, komunikat komunikat) {
    packet_t pakiet_ = przygotujPakiet(nr_tunelu, gdzie, zapisanyZegar);
    for (int i = 0; i < LICZBA_EKIP; i++) {
        if (i != id_proc) {
            MPI_Send(&pakiet_, sizeof(packet_t), MPI_BYTE, i, komunikat, MPI_COMM_WORLD);
            // debug("MPIBroadcast SEND to process %d", i);
        }
    }
}
// Sendlocal do wymiany na pthread_cond_wait
void MPI_SendLocal(komunikat komunikat) {
    MPI_Send(0, sizeof(int), MPI_INT, id_proc, komunikat, MPI_COMM_WORLD); //ID_WATKU_KOM
}

void MPI_RecvLocal(komunikat komunikat) {
    MPI_Status status_;
    int signal;
    debug("TEST id watku kom %d Recv Local ?", ID_WATKU_KOM);
    MPI_Recv(&signal, sizeof(int), MPI_INT, id_proc, komunikat, MPI_COMM_WORLD, &status_); //ID_WATKU_KOM
    debug("TEST RecvLocal OK");
}