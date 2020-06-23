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
};

extern volatile Tunel * tunele;
// extern volatile vector<int> kolejkaDoTunelu;

#endif