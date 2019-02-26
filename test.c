#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(){
	//create a random bytes between 2048 and 20480
	//Using Srand to change the seed so the same random number isnt used each run
	srand(time(NULL));
	int r = rand() % 20480 + 2048;
	int datastart = rand() % 45055; //65535 -20480


	//Allocate a block of memory, this will be 2^16 (Not 2^16-1, can assume full address space)
	//create array to allocate block of memory
	int space = 65536;
	char initialise = 48;//48 = 0 in Ascii table
  unsigned char *allocatemem; 
	allocatemem = malloc(space);

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

	//Page table needs 256 pages
	//Implement via 2D array Rows = Pages, columns =  frame.
	int pagetable[256][2];
	int pagenum = 0;

	int firstframe = datastart / 256;
	int lastframe = (r + datastart) / 256;

	//initialise the page table, 256 rows
	for(int i = 0; i < 256; i++)
	{
		pagetable[i][0] = i;
		pagetable[i][1] = 0;
	}

	for(int j = firstframe; j < lastframe; j++)
	{
		pagetable[pagenum][1] = j;
		pagenum++;
	}
	//Similar to above but for bytes in frames. 512 frames, 128 bytes per frame, 65536 bytes total.
	/*int physicalmem[512][128];
	int bytenum = 0;

	for(int i = 0; i < 512; i++)
	 {
	 	//	printf("Byte num start = %d\n\n", bytenum);
	     for(int x = 0; x < 128; x++)
	     {
	       physicalmem[i][x] = bytenum;
	       bytenum++;
	       if(x == 127)
	       {
						bytenum = 0;
						//printf("Bytenum now = %d", bytenum);
	       }
	     }
	   // printf("Byte num end = %d\n\n", bytenum);
	 }*/

	 //Write to a File
	 int framenum = 0;
	 FILE *file = fopen("./trial.txt", "w");

	 fprintf(file, "Address\t  |\t  Frame\t   |\t Content\n");
	 fprintf(file, "-------- \t|\t --------\t | --------\n");

	 for(int i = 0; i < space; i++)
	 {
	 		if(i % 256 == 0 && i != 0)
	 		{
	 			framenum++;
	 		}
			if(allocatemem[i] == 49)
			{
			fprintf(file, "0x%X  \t| \t   %d    \t|  %c 	\t| process running\n", i, framenum, allocatemem[i]);
			}
			else
			{
			fprintf(file, "0x%X  \t| \t		%d		\t|  %c	\n", i, framenum, allocatemem[i]);
			}
	 }

	 FILE *pagefile = fopen("./pagetable.txt", "w");

	fprintf(pagefile, "Page Number\t  |\t  Frame\n");
  fprintf(pagefile, "-------- \t		|\t -------- \n");
  for(int i = 0; i < 256; i++)
  {
			fprintf(pagefile, "		%d		\t	|\t		%d		\n", pagetable[i][0], pagetable[i][1]);
  }
	 
	printf("The random number generated was: %d and: %d\n", r, datastart);
  //printf("%c \n",  allocatemem[5]);
	//printf("memory allocated: %zu\n ", sizeof(&allocatemem));
	/*for(int i = 0; i < 512; i++)
	{
		printf("Row");
			for(int x = 0; x < 128; x++)
			{
				printf("%d ", pagetable[i][x]);
			}
		printf("\n\n");
	}*/
	free(allocatemem);
	fclose(file);
	fclose(pagefile);
	return 0;
}
