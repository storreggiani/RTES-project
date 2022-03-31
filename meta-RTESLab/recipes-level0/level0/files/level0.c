#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	
	printf("QUESTO PROGRAMMA EFFETTUA LA SOMMA DI 2 VETTORI PASSATI DALL'UTENTE\n\n");
	
	/*Deve essere passato esattamente un parametro*/
	if (argc != 2 ) 
	{
		printf("Errore nel numero di parametri! Bisogna passare un solo parametro: la lunghezza dei vettori.\n");
		exit(0);
	} 
	
	int lun = atoi(argv[1]);
	if(lun <= 0){
		printf("La lunghezza deve essere maggiore di 0!\n");
		exit(1);
	} 

	int vettore1[lun], vettore2[lun], v1, v2, ret[lun];;
    
	for(int i = 0; i < lun; i++){
		printf("Elemento %d del primo vettore:", i);
       		scanf("%d", &v1);
		vettore1[i] = v1;
        }

        for(int i = 0; i < lun; i++){
               printf("Elemento %d del secondo vettore:", i);
                scanf("%d", &v2);
                vettore2[i] = v2;
        }

	for(int i = 0; i < lun; i++){
	       ret[i] = vettore1[i] + vettore2[i];
	}

        printf("Il vettore somma è: [");
        for(int i = 0; i < lun; i++){
	       if (i == lun-1)
		       printf("%d]\n", ret[i]);
	       else
		       printf("%d, ", ret[i]);

        }

        return 0;
}
