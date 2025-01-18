#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

typedef struct{
	time_t t_sec;
	suseconds_t t_usec;
} Time;

int main(){
	Time start, end;
	long seconds, useconds; 
    double elapsed;

    // Temps de début
	printf("Debut du programme\n");
    gettimeofday(&start, NULL);
	sleep(5);
    // Temps de fin
    gettimeofday(&end, NULL);
	printf("Fin du programme\n");
	
    // Calcul de la différence
    seconds = end.t_sec - start.t_sec;
    useconds = end.t_usec - start.t_usec;
    elapsed = seconds + useconds / 1e6;

    printf("Temps écoulé : %.6f secondes\n", elapsed);

	return 0;
}
