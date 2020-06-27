#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

MPI_Status status;
int zapisanyZegar; 
pthread_cond_t PRZEKAZ_RELEASE, PRZEKAZ_INSIDE, PRZEKAZ_ACK; // init
pthread_mutex_t mutex; //init

vector<int> kolejkaDoTunelu = {}; // wspoldzielenie?

kierunki wybranyKierunek = brak;

packet_t pakiet_glowny;

int ret; // kody od pthread_cond_wait

void mainLoop() {
    wybranyKierunek = tam;
    czekajNaWejscie(wybranyKierunek);
    przejdzTunelem(wybranyKierunek);
    krainaSzczesliwosci();
    MPI_Barrier(MPI_COMM_WORLD);
    // wybranyKierunek = zPowrotem;
    // czekajNaWejscie(wybranyKierunek);
    // przejdzTunelem(wybranyKierunek);
    // dojdzDoSiebie();
}

void czekajNaWejscie(kierunki gdzie) {
    zapisanyZegar = zegar;
    debug("Stan zegara: %d", zegar);
    stanBogacza = czekamNaTunel;

    /* Czekam az zwolni sie jakis tunel */
    wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    stanWatku = czekamNaRelease;
    zwiekszZegar();
    // debug("Stan zegara: %d",zegar);
    while /* nie ma miejsca */(wybranyTunel == -1) {
        debug("Nie ma dla mnie tunelu, czekam az ktos wyjdzie");
        // MPI_RecvLocal(PRZEKAZ_RELEASE);
        ret = pthread_cond_wait(&PRZEKAZ_RELEASE, &mutex);
        debug("Ktos wyszedl, sprawdze miejsce");
        wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    }
    stanWatku = ide;
    
    /* Rozglaszam gdzie chcę sie dostać i zbieram odpowiedzi */
    debug("Oglaszam, ze chce do tunelu %d", wybranyTunel);
    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, REQ);
    
    debug("[TEST] oczekujace: %d", oczekujace);

    kolejkaDoTunelu.clear();
    stanWatku = czekamNaAck;
    for /* Odczytaj ACK od oczekujacych */ (int i = 0; i < oczekujace; i++) {
        // MPI_RecvLocal(PRZEKAZ_ACK);
        ret = pthread_cond_wait(&PRZEKAZ_ACK, &mutex);
        debug("TEST Dostalem ACK lokalne");
    }

    /* Czekam az bede mial pierwszenstwo */
    stanWatku = czekamNaInside; //te moglyby byc atomowe
    while(!kolejkaDoTunelu.empty()) {
        debug("Czekam w kolejce do tunelu %d", wybranyTunel);
        // MPI_RecvLocal(PRZEKAZ_INSIDE);
        ret = pthread_cond_wait(&PRZEKAZ_INSIDE, &mutex);
    }
    stanWatku = ide;

    /* Czekam az bede mial miejsce */
    stanWatku = czekamNaRelease;
    while(!sprawdzMiejsceWTunelu(wybranyTunel, gdzie)) {
        debug("Czekam na miejsce w tunelu %d", wybranyTunel);
        // MPI_RecvLocal(PRZEKAZ_RELEASE);
        ret = pthread_cond_wait(&PRZEKAZ_RELEASE, &mutex);
    }
    stanWatku = ide;
}

void przejdzTunelem(kierunki gdzie) {
    zwiekszZegar();
    debug("Stan zegara: %d",zegar);
    stanBogacza = ide;
    debug("JESTEM W TUNELU %d do %d", wybranyTunel, wybranyKierunek); 
    zapisanyZegar = zegar; // czemu to służyło?
    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, INSIDE);

    // mySleep(100000000);

    debug("Zaraz sprawdze czy moge wyjsc");
    while(!tunele[wybranyTunel].kolejkaWTunelu.empty()){
        stanWatku = czekamNaRelease;
        debug("Jeszcze nie moge wyjsc");
        // MPI_RecvLocal(PRZEKAZ_RELEASE);
        ret = pthread_cond_wait(&PRZEKAZ_RELEASE, &mutex);
    }
    debug("moge wyjsc, ide!");

    if (kolejkaDoTunelu.empty()) {
        debug("Koleka jest pusta");
    } else {
     debug("Liczba w kolejce %ld", kolejkaDoTunelu.size());
    }
    stanWatku = ide;

    MPI_Broadcast(wybranyTunel,gdzie,zapisanyZegar,RELEASE);  
}

#include <unistd.h>
void krainaSzczesliwosci() {
    zwiekszZegar();
    debug("Stan zegara: %d",zegar);
    debug("Jestem w krainie szczesliwosci");
   // sleep(100000);
    debug("Koniec spanka");
}

void dojdzDoSiebie() {
    zwiekszZegar();
    debug("Stan zegara: %d",zegar);
    debug("Dochodze do siebie");
    sleep(5);
}


/**
 * Jesli proces jest w kolejce usuwa go z niej (tzn usuwa pierwszy, bo pozniejszy by nie wyslal INSIDE)
 */
void obsluzKolejkeDoTunelu(int obcy_proc_id) {

    if /* vector contains element */ (find(kolejkaDoTunelu.begin(), kolejkaDoTunelu.end(), obcy_proc_id) != kolejkaDoTunelu.end()) {

        /* pop_front */
        kolejkaDoTunelu.front() = std::move(kolejkaDoTunelu.back());
        kolejkaDoTunelu.pop_back();
        debug("%d usuniety z kolejki do tunelu", obcy_proc_id)
    } else {
        debug("RELEASE ale nie bylo w kolejce");
    }
}

bool obcyMaPierwszenstwo(packet_t pakiet_) {
    return pakiet_.proc_zegar > zapisanyZegar || (pakiet_.proc_zegar == zapisanyZegar && pakiet_.proc_id < id_proc);
}

void zwiekszZegar(){
    zegar++;
}