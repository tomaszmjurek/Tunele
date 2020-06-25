#include "main.h"
#include "tunel.h"
#include <algorithm>
Tunel * tunele; /* zmienna globalna */
// vector<int> kolejkaDoTunelu = {};

/**
 * sprawdza najpiew tunel bazujac na id_procesu,
 * aby rozdzielic procesy na wieksza liczbe tuneli
 * potem sprawdza całą tablice
 * @return numer wolnego tunelu lub -1
 */
int znajdzMiejsceWTunelu(kierunki gdzie) {
    int nrTunelu = id_proc % LICZBA_TUNELI;
    if (tunele[nrTunelu].kierunek == brak || tunele[nrTunelu].kierunek == gdzie)
        if (tunele[nrTunelu].zajetosc < POJEMNOSC_TUNELU)
            return nrTunelu;
    for (int i = 0; i < LICZBA_TUNELI; i++) {
        nrTunelu = i;
        if (tunele[nrTunelu].kierunek == brak || tunele[nrTunelu].kierunek == gdzie)
            if (tunele[nrTunelu].zajetosc < POJEMNOSC_TUNELU)
                return nrTunelu;
    }
    return -1;
}

/**
 * sprawdza miejsce w konkretnym tunelu 
 * @return true / false
 */
bool sprawdzMiejsceWTunelu(int nrTunelu, kierunki gdzie) {
    if (tunele[nrTunelu].kierunek == gdzie || tunele[nrTunelu].kierunek == brak)
        if (tunele[nrTunelu].zajetosc + ROZMIAR_EKIPY <= POJEMNOSC_TUNELU)
            return true;
    return false;
}

void dodajDoTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie) {
    if (tunele[nrTunelu].zajetosc + rozmiarEkipy < POJEMNOSC_TUNELU)
        tunele[nrTunelu].zajetosc += rozmiarEkipy;
    
    if (tunele[nrTunelu].kierunek == brak)
        tunele[nrTunelu].kierunek = gdzie;
}

void usunZTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie){
    if (tunele[nrTunelu].zajetosc - rozmiarEkipy > 0)
        tunele[nrTunelu].zajetosc -= rozmiarEkipy;

    if (tunele[nrTunelu].zajetosc == 0)
        tunele[nrTunelu].kierunek = brak;
}

void czyscTunele() {
    for (int i = 0; i < LICZBA_TUNELI; i++) {
        tunele[i].zajetosc = 0;
        tunele[i].kierunek = brak;
    }
}

void kolejkaWTuneluPopFront(int nrTunelu, int obcy_proc_id) {
        /* pop_front */
          if /* vector contains element */ (find(tunele[nrTunelu].kolejkaWTunelu.begin(), tunele[nrTunelu].kolejkaWTunelu.end(), obcy_proc_id) != tunele[nrTunelu].kolejkaWTunelu.end()) {

        tunele[nrTunelu].kolejkaWTunelu.front() = std::move(tunele[nrTunelu].kolejkaWTunelu.back());
        tunele[nrTunelu].kolejkaWTunelu.pop_back();
          }
}

