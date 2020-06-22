#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>
#include <algorithm>

MPI_Status status;
vector<int> kolejkaDoTunelu = {};

packet_t pakiet;

void mainLoop() {
    czekajNaWejscie(tam, zegar);
    przejdzTunelem(tam);
    krainaSzczesliwosci();
    czekajNaWejscie(zPowrotem);
    przejdzTunelem(zPowrotem);
    dojdzDoSiebie();
}

void czekajNaWejscie(kierunki gdzie, int zegar) {
    int zapisanyZegar = zegar;
    stanBogacza = czekamNaTunel;
    int wybranyTunel = znajdzMiejsceWTunelu(gdzie);

    if /* nie ma miejsca */ (wybranyTunel != -1) {
        stanWatku = czekamNaRelease;
        while (stanWatku == czekamNaRelease) {
            // TODO: aktywne czekanie - rozwiazanie najprostrze, moze da się to ulepszyć
        }
        // stanWatku = ide;
        czekajNaWejscie(gdzie, zapisanyZegar);
    } else {
        MPI_Broadcast(wybranyTunel, gdzie, REQ);
        kolejkaDoTunelu.clear();
        for /* Odczytaj ACK od oczekujacych */ (int i = 0; i < oczekujace; i++) {
            MPI_Recv(&pakiet, 40 , MPI_PAKIET_T, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
            if (pakiet.nr_tunelu == wybranyTunel && pakiet.proc_zegar < zegar) {
                kolejkaDoTunelu.push_back(pakiet.proc_id); // rezygnuje z zapisaywanie proc_zegar
                //sort(kolejkaDoTunelu.begin(), kolejkaDoTunelu.end()); // raczej zbedne
            }
        }

        stanWatku = czekamNaInside;
        while(!kolejkaDoTunelu.empty()) {
            // TODO: aktywne czekanie, niedobrze
        }
        stanWatku = ide;

        //else
        //dopoki jest kolejka sluchaj inside i usuwaj
        //masz dostep return, albo koniec funkcji
    }

}

void przejdzTunelem(kierunki gdzie) {
    //MPI_Send(przygotujPakiet(gdzie), 40, MPI_PAKIET_T, BROADCAST, INSIDE, MPI_COMM_WORLD);
    //usun kolejkeDostepu
    sleep(3);
    //jesli jestes pierwszy return
    //else czekaj az sie zwolni (RELEASE)
    //wyslij RELEASE
    
    pakiet = przygotujPakiet(0 /*numer tunelu*/,gdzie); 
    MPI_Send(&pakiet, 40, MPI_PAKIET_T, 0/*BROADCAST!*/, RELEASE, MPI_COMM_WORLD);
}

void krainaSzczesliwosci() {
    debug("Jestem w krainie szczesliwosci");
    sleep(5);
}

void dojdzDoSiebie() {
    debug("Dochodze do siebie");
    sleep(5);
}


/**
 * Jesli proces jest w kolejce usuwa go z niej (tzn usuwa pierwszy, bo pozniejszy by nie wyslal INSIDE)
 */
void obsluzKolejkeDoTunelu(int proc_id) {

    if /* vector contains element */ (find(kolejkaDoTunelu.begin(), kolejkaDoTunelu.end(), proc_id) != kolejkaDoTunelu.end()) {

        /* pop_front */
        kolejkaDoTunelu.front() = std::move(kolejkaDoTunelu.back());
        kolejkaDoTunelu.pop_back();
    }

    if (kolejkaDoTunelu.empty()) {
        // sprawdz czy jest miejsce w tunelu
        // mozesz isc, ale nie do konca, bo co jak zajety tunel
    }
}
