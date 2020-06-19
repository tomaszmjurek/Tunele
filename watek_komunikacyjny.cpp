#include "main.h"
#include <string>
using namespace std;
#include "watek_komunikacyjny.h"
#include "watek_glowny.h"
#include "tunel.h"

volatile int oczekujace;
bool dontStop = true;
stany stan;
komunikaty komunikat;
packet_t pakiet;
MPI_STATUS status;
int wysylajacy;
void *startWatekKom(void *ptr) {
    while (dontStop) {
        debug("Czekam na recv");
        MPI_Recv( &pakiet, 40 , MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        komunikat = MPI_TAG;
        wysylajacy = MPI_SOURCE;
        switch(komunikat) {
            case REQ:
                debug("Otrzymalem REQ...");
<<<<<<< HEAD
                if (stan == czekam) {
                // nie powinniśmy zerować structa?
<<<<<<< HEAD
                    MPI_Send(&pakiet, 40, MPI_PAKIET_T, wysylajacy, ACK,MPI_COMM_WORLD);
                }
=======
                MPI_Send(&pakiet, 40, MPI_PAKIET_T, wysylajacy, ACK,MPI_COMM_WORLD);
>>>>>>> dalszy rozwój watek_komunikacyjny
=======
                if (stan == czekam)
                // nie powinniśmy zerować structa?
                MPI_Send(&pakiet, 40, MPI_PAKIET_T, wysylajacy, ACK,MPI_COMM_WORLD);
>>>>>>> 0101ce42ddabc3addbd631cbcad062f80a6c0c8a
                oczekujace++;
                //wysylajacy moze byc nie tak, czy thread_id bedzie jednoznaczny z rank?
                //co z zwiekszeniem oczekujacy w stanie Zajety?
                break;
            case INSIDE:
                debug("Otrzymalem INSIDE...");
                if (stan == czekam)
                    //usunzkolejkiZadanJesliJest
                oczekujace--;
                //uaktualnijTunele(1)
<<<<<<< HEAD
<<<<<<< HEAD
                dodajDoTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy,pakiet.kierunek);
=======
                dodajDoTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy,pakiet.kierunek );
>>>>>>> dalszy rozwój watek_komunikacyjny
=======
                dodajDoTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy,pakiet.kierunek );
>>>>>>> 0101ce42ddabc3addbd631cbcad062f80a6c0c8a
                //dodanie do kolejki jeśli kierunek =0
                break;
            case RELEASE:
                debug("Otrzymalem RELEASE...");
                //uaktualnijTunele(-1)
<<<<<<< HEAD
<<<<<<< HEAD
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy,pakiet.kierunek);
=======
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy,pakiet.kierunek)
>>>>>>> dalszy rozwój watek_komunikacyjny
=======
                usunZTunelu(pakiet.nr_tunelu, pakiet.rozmiar_grupy,pakiet.kierunek)
>>>>>>> 0101ce42ddabc3addbd631cbcad062f80a6c0c8a
                //warunek z kolejką procesów znów
                break;
            case STOP:
                //zwolnij pamiec, zabij procesy
                dontStop = false;
                terminate();
            default:
                debug("Otrzymalem błędny komunikat");
        }
    }
}