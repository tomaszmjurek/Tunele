#ifndef WATEK_KOMUNIKACYJNY_H
#define WATEK_KOMUNIKACYJNY_H

/* wątek komunikacyjny: odbieranie wiadomości i reagowanie na nie poprzez zmiany stanu */
void *startWatekKom(void *ptr);
packet_t przygotujPakiet(int, kierunki);
void MPI_Broadcast(int nr_tunelu, kierunki gdzie, komunikat komunikat);

#endif
