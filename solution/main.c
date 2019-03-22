#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(){
	//create a random bytes between 2048 and 20480
	//Using Srand to change the seed so the same random number isnt used each run
	srand(time(NULL));
	int r = rand() % 18432 + 2048;//18432 + 2048 = 20480
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
		int ranAscii = rand() % 77 + 49;
		allocatemem[bytestart] = ranAscii;
		bytestart++;
	}

	//Page table needs 256 pages
	//Implement via 2D array. need 256 rows and 2 columns
	//Rows = 256, columns 1 = page number, column 2 = frame number
	int pagetable[256][2];
	int pagenum = 0;

	int firstframe = datastart / 256;
	int lastframe = (r + datastart) / 256;
	//initialise the page table
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
	//2 table entries to be stored on disk instead
	pagetable[pagenum][1] = 1;
	pagetable[pagenum + 1][1] = 2;

	 //Write to a File
	 int framenum = 0;
	 FILE *file = fopen("./data/physical_memory.txt", "w");

	 fprintf(file, "Address\t  |\t  Frame\t   |\t Content\n");
	 fprintf(file, "-------- \t|\t --------\t | --------\n");

	 for(int i = 0; i < space; i++)
	 {
	 		if(i % 256 == 0 && i != 0)
	 		{
	 			framenum++;
	 		}
			if(allocatemem[i] != 48)
			{
			fprintf(file, "0x%X  \t| \t   %d    \t|  %c 	\t| process running(Present bit 1)\n", i, framenum, allocatemem[i]);
			}
			else
			{
			fprintf(file, "0x%X  \t| \t		%d		\t|  %c	\n", i, framenum, allocatemem[i]);
			}
	 }


	//Write page table to a file.
	 FILE *pagefile = fopen("./data/page_table.txt", "w");

	fprintf(pagefile, "Page Number\t  |\t  Frame\n");
  fprintf(pagefile, "-------- \t		|\t -------- \n");
  for(int i = 0; i < 256; i++)
  {
  		if(pagetable[i][1] == 1 || pagetable[i][1] == 2)
  		{
  		fprintf(pagefile, "   %d    \t  |\t   %d    \t| Should be (Frame - 1). This is frame stored on Disk(present bit 0)\n", pagetable[i][0], pagetable[i][1]);
  		}
  		else
  		{
			fprintf(pagefile, "		%d		\t	|\t		%d		\n", pagetable[i][0], pagetable[i][1]);
			}
  }


	//Explanation of translation
	printf("\n\nWhen the user enters a hex address, my program will assign it to an unassigned int variable. This variable is the put through a bitmask to find the offset. This mask will be 0x00FF, FF signifies that I want the last 2 hex values. Because this is a 16bit system the offset will be 8 bytes which is the 2 rightmost hex values in the address. the 2 left values contain the Virtual Page Number (VPN). To get this VPN my program shiifts the bits in the entered addess by 8. This will move the hex characters 2 places to the right, meaning the last 2 will be discarded while the left 2 will take their place. The 2 most hex characters are now set to 0, meaning that we are left with the VPN. My program now uses this VPN to look up the page table. The VPN contains the page I want so I find the page number the VPN equals. This page on the table will point to a frame in physical memory. For example lets say it points at frame 10. In physical memory, frame 10 starts at byte 2560 and ends at byte 2815 in my physical memory simulation. Now I take the offset we calculated earlier and add this to 2560. Now I have the address in physical memory I want, I simply retrieve the data and print it to the user.In the case where the address entered is not in physical memory, the hard disk will be checked. If the address is present in the disk, the data will be swapped to physical memory by changing the present bit, which is 0 in disk, to 1 (Present bit is 1 in physical memory). This data is swapped to the next available space in physical memory.\n\n");


	//Get hexadecimal input from the user
	unsigned int address;
	while(address != -1)
	{
		printf("Please enter the Vitual Memory Address you would like to access (In hexdecimal format)\n");
		scanf("%X", &address);

		//First find the offset using a bit mask
		unsigned int offset_mask = 0x00FF;
		unsigned int offset = address & offset_mask;

		//Next find VPN using 
		unsigned int vpn = address >> 8;

		//Use VPN on page table to find Frame
		int foundFrame;
		for(int i = 0; i < 255; i++)
		{
			if(pagetable[i][0] == vpn)
			{
				foundFrame = pagetable[i][1];
			}
		}

		//Now add the offset to the frame starting byte (Framenum * 256) + offset)
		int findaddress =  (foundFrame * 256) + offset;
		printf("The data at the entered address in physical memory is: %c\n", allocatemem[findaddress]);
	}


	//printf("The address entered was: %d", address);
	//printf("The random number generated was: %d and: %d\n", r, datastart);
  //printf("%c \n",  allocatemem[5]);
	//printf("memory allocated: %zu\n ", sizeof(&allocatemem));
	free(allocatemem);
	fclose(file);
	fclose(pagefile);
	return 0;
}
