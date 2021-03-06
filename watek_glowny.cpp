#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>
#include <stdio.h>
#include <algorithm>

MPI_Status status;
int zapisanyZegar; 
pthread_cond_t PRZEKAZ_RELEASE, PRZEKAZ_INSIDE, PRZEKAZ_ACK;
pthread_mutex_t mutex;

vector<int> kolejkaDoTunelu = {};

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
    stanWatku = czekamNaAck;
    MPI_Broadcast(wybranyTunel, gdzie, zapisanyZegar, REQ);
    kolejkaDoTunelu.clear();
    
    debug("Liczba oczekujacych: %d", oczekujace);
    for /* Odczytaj ACK od oczekujacych */ (int i = 0; i < oczekujace; i++) {
        debug("Czekam na ACK");
        ret = pthread_cond_wait(&PRZEKAZ_ACK, &mutex);
        debug("Dostałem ACK iteracja %d/%d", i+1, oczekujace);
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
    debug("JESTEM W TUNELU %s kierunek %d zegar %d", (wybranyTunel == 0 ? "tam" : "zPowrotem"), wybranyKierunek, zegar); 
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
    }
}

bool obcyMaPierwszenstwo(packet_t pakiet_) {
    return pakiet_.proc_zegar > zapisanyZegar || (pakiet_.proc_zegar == zapisanyZegar && pakiet_.proc_id < id_proc);
}

void zwiekszZegar() {
    zegar++;
}