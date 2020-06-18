#include "main.h"
#include "tunel.h"

// class Tunel {
//     public:
//         int zajetosc;
//         kierunki kierunek;
// };

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