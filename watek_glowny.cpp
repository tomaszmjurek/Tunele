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
int oczekujace_lokalnie;
void mainLoop() {
    while (true) {
        wybranyKierunek = tam;
        czekajNaWejscie(wybranyKierunek);
        przejdzTunelem(wybranyKierunek);
        krainaSzczesliwosci();
        MPI_Barrier(MPI_COMM_WORLD);
        wybranyKierunek = zPowrotem;
        czekajNaWejscie(wybranyKierunek);
        przejdzTunelem(wybranyKierunek);
        dojdzDoSiebie();
    }
}

void czekajNaWejscie(kierunki gdzie) {
    zapisanyZegar = zegar;
    debug("Stan zegara: %d", zegar);
    stanBogacza = czekamNaTunel;

    /* Czekam az zwolni sie jakis tunel */
    wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    stanWatku = czekamNaRelease;
    zwiekszZegar();
    while /* nie ma miejsca */(wybranyTunel == -1) {
        debug("Nie ma dla mnie tunelu, czekam az ktos wyjdzie");
        ret = pthread_cond_wait(&PRZEKAZ_RELEASE, &mutex);
        debug("Ktos wyszedl, sprawdze miejsce");
        wybranyTunel = znajdzMiejsceWTunelu(gdzie);
    }
    stanWatku = ide;
    
    /* Rozglaszam gdzie chcę sie dostać i zbieram odpowiedzi */
    debug("Oglaszam, ze chce do tunelu %d, zapisany zegar: %d", wybranyTunel, zapisanyZegar);

    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, REQ);

    kolejkaDoTunelu.clear();
    stanWatku = czekamNaAck;
    debug("Liczba oczekujacych: %d", oczekujace);
    // oczekujace_lokalnie = oczekujace;
    for /* Odczytaj ACK od oczekujacych */ (int i = 1; i <= oczekujace; i++) {
        debug("Czekam na ACK");
        ret = pthread_cond_wait(&PRZEKAZ_ACK, &mutex);
        debug("Dostałem ACK numer %d/%d", i, oczekujace);
    }

    /* Czekam az bede mial pierwszenstwo */
    
    stanWatku = czekamNaInside; 
    while(!kolejkaDoTunelu.empty()) {
        debug("Czekam w kolejce do tunelu %d", wybranyTunel);
        ret = pthread_cond_wait(&PRZEKAZ_INSIDE, &mutex);
    }
    
    stanWatku = ide;

    /* Czekam az bede mial miejsce */
    stanWatku = czekamNaRelease;
    while(!sprawdzMiejsceWTunelu(wybranyTunel, gdzie)) {
        debug("Czekam na miejsce w tunelu %d", wybranyTunel);
        ret = pthread_cond_wait(&PRZEKAZ_RELEASE, &mutex);
    }
    stanWatku = ide;
}

void przejdzTunelem(kierunki gdzie) {
    zwiekszZegar();
    stanBogacza = ide;
    debug("JESTEM W TUNELU %d do %d zegar %d", wybranyTunel, wybranyKierunek, zegar); 
    MPI_Broadcast(wybranyTunel, gdzie, zegar, INSIDE);
    dodajSiebieDoTunelu(wybranyTunel);
    sleep(2);
    stanWatku = czekamNaRelease;
    debug("Zaraz sprawdze czy moge wyjsc");
    while(!czyJestemPierwszyWTunelu(wybranyTunel)) {
        debug("Jeszcze nie moge wyjsc");
        ret = pthread_cond_wait(&PRZEKAZ_RELEASE, &mutex);
    }
    kolejkaWTuneluPopFront(wybranyTunel, id_proc);
    stanWatku = ide;
    debug("Moge wyjsc, ide!");
    MPI_Broadcast(wybranyTunel, gdzie, zegar,RELEASE);  
}

// #include <unistd.h>
void krainaSzczesliwosci() {
    zwiekszZegar();
    debug("Jestem w krainie szczesliwosci! Zegar: %d", zegar);
    sleep(4);
}

void dojdzDoSiebie() {
    zwiekszZegar();
    debug("Dochodze do siebie... zegar: %d", zegar);
    sleep(4);
    debug("Koniec spanka, chce do krainy");
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