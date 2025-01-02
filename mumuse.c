#include <stdlib.h>
#include <stdio.h>

// Manip' bit a bit
int main(int argc, char **argv){
	int	num = atoi(argv[1]);
	printf("num & (1 << 31) = %i\n", num & (1 << 31));
	if (num & (1 << 31))
		printf("Negatif\n");
	else
		printf("Positif\n");
	return 0;
}

