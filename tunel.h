#ifndef TUNEL_H
#define TUNEL_H
#include <vector>
using namespace std;

int znajdzMiejsceWTunelu(kierunki gdzie);
bool sprawdzMiejsceWTunelu(int nrTunelu, kierunki gdzie);
void dodajDoTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie);
void usunZTunelu(int nrTunelu, int rozmiarEkipy, kierunki gdzie);
void czyscTunele();
class Tunel {
    public:
        int zajetosc;
        kierunki kierunek;
        vector<int> kolejkaWTunelu;
};
// extern vector<int> kolejkaWTunelu[5] = {};//trzeba wykombinowac cos z tym argumentem, nie moze byc LICZBA_TUNELi, ale musi byc liczba tuneli XD

extern Tunel * tunele;
// extern volatile vector<int> kolejkaDoTunelu;

void kolejkaWTuneluPopFront(int nrTunelu, int obcy_proc_id);

void sprawdzCzyPierwszy(packet_t pakiet);

#endif