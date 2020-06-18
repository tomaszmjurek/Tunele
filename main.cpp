#include "main.h"
#include "watek_glowny.h"
#include "watek_komunikacyjny.h"
#include <pthread.h>
#include "tunel.h"

using namespace std;

pthread_t threadKom, threadBogacz;

int main(int argc, char *argv[]) {
    
    if (argc > 0) { // == 5
        LICZBA_TUNELI = atoi(argv[1]);
        POJEMNOSC_TUNELU = atoi(argv[2]); //wieksza niz min 1 ekipa
        LICZBA_EKIP = atoi(argv[3]);
    } else {
        LICZBA_TUNELI = 5;
        POJEMNOSC_TUNELU = 40;
        LICZBA_EKIP = 15;
    }

    tunele = new Tunel [LICZBA_TUNELI];

    ROZMIAR_EKIPY = rand() % 21 + 10;
    
    if (POJEMNOSC_TUNELU < ROZMIAR_EKIPY) {
        cout << "Pojemność tunelu za mała, zalecane > 40" << endl;
        return 1;
    }

    pthread_create(&threadKom, NULL, startWatekKom, 0);

    //tworz watki

    return 0;
}