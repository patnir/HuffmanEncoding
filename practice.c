#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	long long int *a = (long long int *)malloc(sizeof(long long int) *10);
	int i = 0;
	for (i = 0; i < 10; i++) {
		a[i] = i * 10;	
	}

	for(i = 0; i < 10; i++){
		printf("%lld\n", a[i]);	
	}


	FILE *fptr = fopen("well.txt", "w+");

	fwrite(a, sizeof(long long int), 10, fptr);

	for (i = 0; i < 10; i++) {
		a[i] = i * 100;	
	}

	fwrite(a, sizeof(long long int), 10, fptr);

	fclose(fptr);

	long long int *b = (long long int *)malloc(sizeof(long long int) * 20);
	
	fptr = fopen("well.txt", "r");

	fread(b, sizeof(long long int), 20, fptr);

	for(i = 0; i < 20; i++){
		printf("%lld\n", b[i]);	
	}

	free(a);
	free(b);

	fclose(fptr);

	return EXIT_SUCCESS;
}
