
/* This program prints out the arguments */

#include <stdio.h>
#include <string.h>

int main (int argc, char** argv) {
	int i;
	
	printf("Hello world.  Your arguments were: \n");
	for (i = 0; i < argc; i ++) {
		printf("%d: %s\n", i, argv[i]);
	}
	printf("Good bye, world!\n");
	return 0;
}

