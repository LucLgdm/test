#include <stdlib.h>
#include <stdio.h>

// Manip' bit a bit
int main(int argc, char **argv){
	// Connaitre le signe d'un nombre
	int	num = atoi(argv[1]);
	if (num & (1 << 31))
		printf("Negatif\n");
	else
		printf("Positif\n");

	// Intervertir 2 nombres
	int a = 10, b = 15;
	printf("a = %i, b = %i\n", a, b);
	a = a^b;
	b = a^b;
	a = a^b;
	printf("a = %i, b = %i\n", a, b);
	
	return 0;
}
