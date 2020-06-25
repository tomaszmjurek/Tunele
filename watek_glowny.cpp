#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>
#include <algorithm>

MPI_Status status;
vector<int> kolejkaDoTunelu = {}; // wspoldzielenie?

int zapisanyZegar;
kierunki wybranyKierunek = brak;

packet_t pakiet_glowny;

void mainLoop() {
    wybranyKierunek = tam;
    czekajNaWejscie(wybranyKierunek);
    przejdzTunelem(wybranyKierunek);
    // krainaSzczesliwosci();
    // wybranyKierunek = zPowrotem;
    // czekajNaWejscie(wybranyKierunek);
    // przejdzTunelem(wybranyKierunek);
    // dojdzDoSiebie();
}

void czekajNaWejscie(kierunki gdzie) {
    zapisanyZegar = zegar;
    stanBogacza = czekamNaTunel;

    /* Czekam az zwolni sie jakis tunel */
    wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    stanWatku = czekamNaRelease;
    while /* nie ma miejsca */(wybranyTunel == -1) {
        debug("Nie ma dla mnie tunelu, czekam az ktos wyjdzie");
        MPI_RecvLocal(PRZEKAZ_RELEASE);
        wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    }
    stanWatku = ide;
    
    /* Rozglaszam gdzie chcę sie dostać i zbieram odpowiedzi */
    debug("Oglaszam, ze chce do tunelu %d", wybranyTunel);
    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, REQ);
    kolejkaDoTunelu.clear();
    for /* Odczytaj ACK od oczekujacych */ (int i = 0; i < oczekujace; i++) {
        MPI_Recv(&pakiet_glowny, sizeof(packet_t), MPI_BYTE, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
        if (pakiet_glowny.nr_tunelu == wybranyTunel && obcyMaPierwszenstwo(pakiet_glowny)) {
            kolejkaDoTunelu.push_back(pakiet_glowny.proc_id); // rezygnuje z zapisaywanie proc_zegar
            debug("No co Pan sie wpycha");
            //sort(kolejkaDoTunelu.begin(), kolejkaDoTunelu.end()); // raczej zbedne
        }
    }

    /* Czekam az bede mial pierwszenstwo */
    stanWatku = czekamNaInside;
    while(!kolejkaDoTunelu.empty()) {
        debug("Czekam w kolejce do tunelu %d", wybranyTunel);
        MPI_RecvLocal(PRZEKAZ_INSIDE);
    }
    stanWatku = ide;

    /* Czekam az bede mial miejsce */
    stanWatku = czekamNaRelease;
    while(!sprawdzMiejsceWTunelu(wybranyTunel, gdzie)) {
        debug("Czekam na miejsce w tunelu %d", wybranyTunel);
        MPI_RecvLocal(PRZEKAZ_RELEASE);
    }
    stanWatku = ide;
}

void przejdzTunelem(kierunki gdzie) {
    stanBogacza = ide;
    debug("JESTEM W TUNELU %d do %d", wybranyTunel, wybranyKierunek); //tu 0
    zapisanyZegar = zegar;
    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, INSIDE);
    // while(kolejkaWTunelu[pakiet_glowny.nr_tunelu].front()!= id_proc){
    //     stanWatku = czekamNaRelease;// to wystarczy? coś śmierdzi
    // }
    // pakiet_glowny = przygotujPakiet(wybranyTunel,wybranyKierunek,gdzie);
    // MPI_Broadcast(wybranyTunel,gdzie,zapisanyZegar,RELEASE);    
}
    
    //jesli jestes pierwszy return
    //else czekaj az sie zwolni (RELEASE)
    //wyslij RELEASE
    
    // pakiet = przygotujPakiet(0 /*numer tunelu*/,gdzie); 
    // MPI_Send(&pakiet, 40, MPI_PAKIET_T, 0/*BROADCAST!*/, RELEASE, MPI_COMM_WORLD);

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

bool obcyMaPierwszenstwo(packet_t pakiet_) {
    return pakiet_.proc_zegar > zapisanyZegar || (pakiet_.proc_zegar == zapisanyZegar && pakiet_.proc_id < id_proc);
}
