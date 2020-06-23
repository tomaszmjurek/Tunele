#ifndef WATEK_KOMUNIKACYJNY_H
#define WATEK_KOMUNIKACYJNY_H

/* wątek komunikacyjny: odbieranie wiadomości i reagowanie na nie poprzez zmiany stanu */
void *startWatekKom(void *ptr);
packet_t przygotujPakiet(int nr_tunelu, kierunki gdzie, int zegar);
void MPI_Broadcast(int nr_tunelu, kierunki gdzie, int zegar, komunikat komunikat);
void MPI_RecvLocal(komunikat komunikat);
void MPI_SendLocal(komunikat komunikat);

extern int oczekujace;

#endif
