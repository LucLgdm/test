#include <stdlib.h>
#include <stdio.h>

void print_bit(int number){
	int i;
	for (i = 7; i >= 0; i--){
		if (number & (1 << i))
			printf("1");
		else
			printf("0");
	}
	printf("\n");
}

// Manip' bit a bit
int main(int argc, char **argv){
	// Connaitre le signe d'un nombre
	printf("Donne le signe du nombre\n");
	int	num = atoi(argv[1]);
	if (num & (1 << 31))
		printf("%i est negatif\n", num);
	else
		printf("%i est positif\n", num);
	printf("\nSwap a et b\n");
	// Intervertir 2 nombres
	int a = atoi(argv[2]), b = atoi(argv[3]);
	printf("a = %i : ", a); print_bit(a);
	printf("b = %i : ", b); print_bit(b);
	a = a^b;
	printf("a = "); print_bit(a);
	b = a^b;
	printf("b = "); print_bit(b);
	a = a^b;
	printf("a = "); print_bit(a);
	printf("a = %i, b = %i\n", a, b);
	
	return 0;
}

