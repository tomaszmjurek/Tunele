#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>

int zegar;
int zapisanyZegar;
// int oczekujace;
//kierunki_t kierunek; //?

//kolejkaDostepu
//bool czekaj wspoldzielony z komunikacyjnym

void mainLoop() {
    czekajNaWejscie(tam);
    przejdzTunelem(tam);
    krainaSzczesliwosci();
    czekajNaWejscie(zPowrotem);
    przejdzTunelem(zPowrotem);
    dojdzDoSiebie();
}

void czekajNaWejscie(kierunki gdzie) {
    zapisanyZegar = zegar;
    // kierunek = gdzie;//?
    if (znajdzMiejsceWTunelu(gdzie) != -1) {
        //czekaj na release
        //zaktualizuj tunele
        //sprawdz tunele (czekajNaWejscie(k)), ale zegar
        //moze jakis return
    } else {
        //odczytaj ACK od oczekujacej procesy
        //if masz dostep -> return (przejdzTunelem)
        //else
        //dopoki jest kolejka sluchaj inside i usuwaj
        //masz dostep return, albo koniec funkcji
    }

}

void przejdzTunelem(kierunki gdzie) {
    //wyslij inside
    //usun kolejkeDostepu
    //Sleep(100);
    //jesli jestes pierwszy return
    //else czekaj az sie zwolni (RELEASE)
    //wyslij RELEASE
}

void krainaSzczesliwosci() {
    sleep(5);
}

void dojdzDoSiebie() {
    sleep(5);
}

