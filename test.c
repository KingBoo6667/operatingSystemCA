#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(){
	//create a random bytes between 2048 and 20480
	//Using Srand to change the seed so the same random number isnt used each run
	srand(time(NULL));
	int r = rand() % 20480 + 2018;
	int datastart = rand() % 45055; //65535 -20480


	//Allocate a block of memory, this will be 2^16 (Not 2^16-1, can assume full address space)
	//create array to allocate block of memory
	int space = 65536;
	char initialise = 48;//48 = 0 in Ascii table
  char *allocatemem; 
	allocatemem = malloc(space * sizeof(char));

	for(int i = 0; i < space; i++)
	{
	allocatemem[i] = initialise;
	}
	//error check
	if (allocatemem == NULL)
	{
		printf("There was a problem allocating memory");
		return 1;
	}

	//Set where the random num of bytes will go
	//Random bytes set to 1 (Ascii 49 = 1)
	for(int i = 0; i < r; i++)
	{
		allocatemem[datastart] = 49;
		datastart++;
	}

	//Page table needs 256 pages w/2frames per page i.e 512 frames
	//Implement via 2D array Rows = Pages columns =  frame. eg. Page 5 frame 10 (to get it 5 * 2)
	// Frame 11 would be (5 * 2) + 1. Basically (page num * 2) ([+1] if frame is odd num)
	int pagetable[256][2];

	//initialise page table
	for(int i = 0; i < 256; i++)
	{
		for(int x = 0; x < 2; x++)
		{
			if(x == 0)
			{
				pagetable[i][x] = i * 2;
			}
			else
			{
				pagetable[i][x] = (i * 2) +1;
			}
		}
	}


	//printf("The random number generated was: %d and: %d\n", r, datastart);
  //printf("%s\n", allocatemem);
	//printf("memory allocated: %zu\n ", sizeof(&allocatemem));
	for(int i = 0; i < 256; i++)
	{
		
			for(int x = 0; x < 2; x++)
			{
				printf("%d \t", pagetable[i][x]);
			}
		printf("\n");
	}
	free(allocatemem);
	return 0;
}
