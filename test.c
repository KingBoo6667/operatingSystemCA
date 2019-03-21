#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(){
	//create a random bytes between 2048 and 20480
	//Using Srand to change the seed so the same random number isnt used each run
	srand(time(NULL));
	int r = rand() % 5120 +512; //int = 4 bytes 20480/4 = 5120 2148/4=512 


	//Allocate a block of memory, this will be 2^16 (Not 2^16-1, can assume full address space)
	//create array to allocate block of memory
	int space = 65535;
	char initialise = 48;
  char *allocatemem; //48 = 0 in Ascii table
	allocatemem = malloc(space * sizeof(char));
	
	for(int i = 0; i < 65535; i++)
	{
	allocatemem[i] = initialise;
	}

	if (allocatemem == NULL)
	{
		printf("There was a problem allocating memory");
		return 1;
	}

	//printf("The random number generated was: %d\n", r);
  //printf("address space: %d\n", addspace);
	//printf("memory allocated: %zu\n ", sizeof(&allocatemem));
	
	free(allocatemem);
	return 0;
}
