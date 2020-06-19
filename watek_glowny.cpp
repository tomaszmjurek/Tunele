#include "main.h"
#include "watek_glowny.h"
#include "tunel.h"
#include "watek_komunikacyjny.h"
#include <unistd.h>

int zegar;
MPI_Status status;
// int zapisanyZegar;
// int oczekujace;
//kierunki_t kierunek; //?

//kolejkaDostepu
//bool czekaj wspoldzielony z komunikacyjnym

packet_t pakiet;

void mainLoop() {
    czekajNaWejscie(tam);
    przejdzTunelem(tam);
    krainaSzczesliwosci();
    czekajNaWejscie(zPowrotem);
    przejdzTunelem(zPowrotem);
    dojdzDoSiebie();
}

void czekajNaWejscie(kierunki gdzie) {
    int zapisanyZegar = zegar;
    stan = czekam;
    int wybranyTunel = znajdzMiejsceWTunelu(gdzie);

    if /* nie ma miejsca */ (wybranyTunel != -1) {
        //czekaj na release MPIrcv albo info do watku kom
        //zaktualizuj tunele
        //sprawdz tunele (czekajNaWejscie(k)), ale zegar
        //moze jakis return
    } else {
        MPI_Broadcast(wybranyTunel, gdzie, REQ);
        int kolejka = 0; //vector
        for (int i = 0; i < oczekujace; i++) {
            MPI_Recv(&pakiet, 40 , MPI_PAKIET_T, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
            if (pakiet.nr_tunelu == wybranyTunel && pakiet.proc_zegar < zegar) {
                // dodajDoKolejkiDostepu
                kolejka++;
            }
        }

        if(kolejka == 0) {

        }
        //odczytaj ACK od oczekujacej procesy
        //if masz dostep -> return (przejdzTunelem)
        //else
        //dopoki jest kolejka sluchaj inside i usuwaj
        //masz dostep return, albo koniec funkcji
    }

}

void przejdzTunelem(kierunki gdzie) {
    //MPI_Send(przygotujPakiet(gdzie), 40, MPI_PAKIET_T, BROADCAST, INSIDE, MPI_COMM_WORLD);
    //usun kolejkeDostepu
    sleep(3);
    //jesli jestes pierwszy return
    //else czekaj az sie zwolni (RELEASE)
    //wyslij RELEASE
    
    pakiet = przygotujPakiet(0 /*numer tunelu*/,gdzie); 
    MPI_Send(&pakiet, 40, MPI_PAKIET_T, 0/*BROADCAST!*/, RELEASE, MPI_COMM_WORLD);
}

void krainaSzczesliwosci() {
    debug("Jestem w krainie szczesliwosci");
    sleep(5);
}

void dojdzDoSiebie() {
    debug("Dochodze do siebie");
    sleep(5);
}

