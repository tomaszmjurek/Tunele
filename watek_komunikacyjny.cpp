#include "main.h"
#include <string>
using namespace std;
#include "watek_komunikacyjny.h"

volatile int oczekujace;
bool dontStop = true;
stany stan;
komunikaty komunikat;

void *startWatekKom(void *ptr) {
    while (dontStop) {
        debug("Czekam na recv");
        // MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //komunikat = MPI_TAG?
        switch(komunikat) {
            case REQ:
                debug("Otrzymalem REQ...");
                if (stan == czekam)
                    //wyslij ACK(zegar, nr_tunelu)
                oczekujace++;
                break;
            case INSIDE:
                debug("Otrzymalem INSIDE...");
                if (stan == czekam)
                    //usunzkolejkiZadanJesliJest
                oczekujace--;
                //uaktualnijTunele(1)
                break;
            case RELEASE:
                debug("Otrzymalem RELEASE...");
                //uaktualnijTunele(-1)
                break;
            case STOP:
                //zwolnij pamiec, zabij procesy
                dontStop = false;
            default:
                debug("Otrzymalem błędny komunikat");
        }
    }
}
