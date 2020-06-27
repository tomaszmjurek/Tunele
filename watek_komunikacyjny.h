#ifndef WATEK_KOMUNIKACYJNY_H
#define WATEK_KOMUNIKACYJNY_H

/* wątek komunikacyjny: odbieranie wiadomości i reagowanie na nie, odblokowanie watku_glownego kiedy czeka */
void *startWatekKom(void *ptr);
packet_t przygotujPakiet(int nr_tunelu, kierunki gdzie, int zegar);
void MPI_Broadcast(int nr_tunelu, kierunki gdzie, int zegar, komunikat komunikat);

extern int oczekujace;
extern int przekazaneACK;
extern bool dontStop;

#endif
