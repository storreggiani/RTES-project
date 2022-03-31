#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int N_CORRIDORI = 0;

struct corsa_t {
    pthread_mutex_t mutex;
    pthread_cond_t arbitro, giocatore, fine;

    int primo, ultimo, cb, stato;
}corsa;

void init_corsa(struct corsa_t* s) {
    pthread_mutexattr_t ma;
    pthread_condattr_t ca;

    pthread_mutexattr_init(&ma);
    pthread_condattr_init(&ca);

    pthread_mutex_init(&s->mutex, &ma);
    pthread_cond_init(&s->arbitro, &ca);
    pthread_cond_init(&s->giocatore, &ca);
    pthread_cond_init(&s->fine, &ca);

    s->primo = s->ultimo = -1;
    s->cb = 0;
    s->stato = 0; // stato = 1 sono arrivati tutti
}

void corridore_attendivia(struct corsa_t* c, int numerocorridore) {
    pthread_mutex_lock(&c->mutex);

    printf("Sono il corridore %d e attendo il via\n", numerocorridore);
    c->cb++;

    if(c->cb == N_CORRIDORI)
        pthread_cond_signal(&c->arbitro);

    while (c->stato == 0)
        pthread_cond_wait(&c->giocatore, &c->mutex);


    pthread_mutex_unlock(&c->mutex);
}

void corridore_arrivo(struct corsa_t* c, int numerocorridore) {
    pthread_mutex_lock(&c->mutex);

    // sono il primo?
    if (c->primo == -1) // si
        c->primo = numerocorridore;

    c->cb--;

    // sono l'ultimo?
    if (c->cb == 0 && c->ultimo == -1) { // si
        c->ultimo = numerocorridore;
        pthread_cond_signal(&c->fine);
    }

    pthread_mutex_unlock(&c->mutex);
}

void arbitro_attendicorridori(struct corsa_t* c) {
    pthread_mutex_lock(&c->mutex);

    while (c->cb != N_CORRIDORI)
        pthread_cond_wait(&c->arbitro, &c->mutex);

    printf("\nArbitro: Corridori pronti\n");

    pthread_mutex_unlock(&c->mutex);
}

void arbitro_via(struct corsa_t* c) {
    pthread_mutex_lock(&c->mutex);

    printf("Arbitro: VIA!!\n");

    c->stato = 1;
    // sveglio tutti i corridori
    pthread_cond_broadcast(&c->giocatore);

    pthread_mutex_unlock(&c->mutex);
}

void arbitro_risultato(struct corsa_t* c) {
    pthread_mutex_lock(&c->mutex);

    while (c->primo == -1 || c->ultimo == -1) {   // da && a ||
        pthread_cond_wait(&c->fine, &c->mutex);
    }

    // corsa finita
    printf("\nIl primo arrivato e' : %d e l'ultimo e' : %d\n\n", c->primo, c->ultimo);

    pthread_mutex_unlock(&c->mutex);
}

void* corridore(void* arg) {
    int numerocorridore = (int)arg;
    //printf("C %d: VADO ALLA PISTA...\n", numerocorridore);
    corridore_attendivia(&corsa, numerocorridore);
    sleep(1); // simulo la corsa con una sleep. Avrei potuto usare anche la solita funzione pausetta
    //printf("%d : CORRO PIU' VELOCE POSSIBILE...\n", numerocorridore);
    corridore_arrivo(&corsa, numerocorridore);
    //printf("%d : TORNO A CASA...\n", numerocorridore);
}

void* arbitro(void* arg) {
    printf("Arbitro: VADO ALLA PISTA...\n");
    arbitro_attendicorridori(&corsa);
    printf("Arbitro: PRONTI, PARTENZA...\n");
    arbitro_via(&corsa);
    printf("Arbitro: ATTENDO CHE ARRIVINO AL TERMINE...\n");
    arbitro_risultato(&corsa);
}



int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("E' richiesto un solo parametro, cioè il numero di corridori presenti alla gara\n");
        exit(NULL);
    }

    N_CORRIDORI = atoi(argv[1]);

    if (N_CORRIDORI < 2)
    {
        printf("Ci devono essere almeno 2 corridori.\n");
        exit(NULL);
    }


    pthread_attr_t a;
    pthread_t ar, g;
    int i;

    /* inizializzo sistema */
    init_corsa(&corsa);

    pthread_attr_init(&a);

    pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

    for (i = 0; i < N_CORRIDORI; i++)
        pthread_create(&g, &a, corridore, (void*)i + 1);

    pthread_create(&ar, &a, arbitro, NULL);


    pthread_attr_destroy(&a);

    /* aspetto 5 secondi prima di terminare tutti quanti */
    sleep(5);

    return 0;
}

