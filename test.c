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
	//initialise the physical memory
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
	int bytestart = datastart;
	for(int i = 0; i < r; i++)
	{
		allocatemem[bytestart] = 49;
		bytestart++;
	}

	//Page table needs 256 pages
	//Implement via 2D array. need 256 rows and 2 columns
	//Rows = 256, columns 1 = page number, column 2 = frame number
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

	//Get hexadecimal input from the user
	unsigned int address;
	printf("Please enter the Vitual Memory Address you would like to access (In hexdecimal format)\n");
	scanf("%X", &address);

	//First find the offset using a bit mask
	unsigned int offset_mask = 0x00FF;
	unsigned int offset = address & offset_mask;
	printf("\nThis address in physical memory returns: %d\n", offset);

	//Next find VPN using 
	unsigned int vpn = address >> 8;
	printf("\nThis address in physical memory returns: %d\n", vpn);

	//Use VPN on page table to find Frame
	int foundFrame;
	for(int i = 0; i < 255; i++)
	{
		if(pagetable[i][0] == vpn)
		{
				foundFrame = pagetable[i][1];
		}
	}
	printf("The frame found was: %d\n", foundFrame);

	//Now add the offset to the frame starting byte (Framenum * 256) + offset)
	int findaddress =  (foundFrame * 256) + offset;
	printf("The data at the entered address in physical memory is: %d ==  %c\n",findaddress, allocatemem[findaddress]);

	//Looped Hexadecial input from user
	/*
	while(address != -1){
		printf("Please enter the Vitual Memory Address you would like to access (In hexdecimal format)\n");
		scanf("%X", &address);
		printf("\nThis address in physical memory returns: %c\n", allocatemem[address]);
	}*/


	//printf("The address entered was: %d", address);
	//printf("The random number generated was: %d and: %d\n", r, datastart);
  //printf("%c \n",  allocatemem[5]);
	//printf("memory allocated: %zu\n ", sizeof(&allocatemem));
	free(allocatemem);
	fclose(file);
	fclose(pagefile);
	return 0;
}
