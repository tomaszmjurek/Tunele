#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "main.h"
using namespace std;


int main(int argc, char *argv[]) {
    
    if (argc > 0) { // == 5
        LICZBA_TUNELI = (int) argv[1];
        POJEMNOSC_TUNELU = (int) argv[2]; //wieksza niz min 1 ekipa
        LICZBA_EKIP = (int) argv[3];
    } else {
        LICZBA_TUNELI = 5;
        POJEMNOSC_TUNELU = 40;
        LICZBA_EKIP = 15;
    }

    tunele = new Tunel [LICZBA_TUNELI];

    ROZMIAR_EKIPY = rand() % 21 + 10;
    
    if (POJEMNOSC_TUNELU < ROZMIAR_EKIPY) {
        cout << "Pojemność tunelu za mała, zalecane > 40" << endl;
        return 0;
    }

    //tworz watki

    return 0;
}