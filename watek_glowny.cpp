#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>
#include <algorithm>

MPI_Status status;
vector<int> kolejkaDoTunelu = {}; // wspoldzielenie?

int zapisanyZegar = 0, wybranyTunel = -1;

packet_t pakiet;

void mainLoop() {
    wybranyKierunek = tam;
    czekajNaWejscie(wybranyKierunek);
    przejdzTunelem(wybranyKierunek);
    krainaSzczesliwosci();
    wybranyKierunek = zPowrotem;
    czekajNaWejscie(wybranyKierunek);
    przejdzTunelem(wybranyKierunek);
    dojdzDoSiebie();
}

void czekajNaWejscie(kierunki gdzie) {
    zapisanyZegar = zegar;
    stanBogacza = czekamNaTunel;

    /* Czekam az zwolni sie jakis tunel */
    int wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    stanWatku = czekamNaRelease;
    while /* nie ma miejsca */(wybranyTunel == -1) {
        MPI_RecvLocal(PRZEKAZ_RELEASE);
        wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    }
    stanWatku = ide;
    
    /* Rozglaszam gdzie chcę sie dostać i zbieram odpowiedzi */
    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, REQ);
    kolejkaDoTunelu.clear();
    for /* Odczytaj ACK od oczekujacych */ (int i = 0; i < oczekujace; i++) {
        MPI_Recv(&pakiet, 40 , MPI_PAKIET_T, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
        if (pakiet.nr_tunelu == wybranyTunel && pakiet.proc_zegar < zegar) {
            kolejkaDoTunelu.push_back(pakiet.proc_id); // rezygnuje z zapisaywanie proc_zegar
            //sort(kolejkaDoTunelu.begin(), kolejkaDoTunelu.end()); // raczej zbedne
        }
    }

    /* Czekam az bede mial pierwszenstwo */
    stanWatku = czekamNaInside;
    while(!kolejkaDoTunelu.empty()) {
        debug("stanBogacza: $s stanWatku: %s", stanBogacza, stanWatku);
        MPI_RecvLocal(PRZEKAZ_INSIDE);
    }
    stanWatku = ide;

    /* Czekam az bede mial miejsce */
    stanWatku = czekamNaRelease;
    while(!sprawdzMiejsceWTunelu(wybranyTunel, gdzie)) {
        debug("stanBogacza: $s stanWatku: %s", stanBogacza, stanWatku);
        MPI_RecvLocal(PRZEKAZ_RELEASE);
    }
    stanWatku = ide;
}

void przejdzTunelem(kierunki gdzie) {
    stanBogacza = ide;
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
}
