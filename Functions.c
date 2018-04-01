#include"main.h"
#include<string.h>
#include<ctype.h>
#include<errno.h>

//Global Structure Variables
Block_bits_t **Data_cache_block;
Block_bits_t **Instruction_cache_block;

void FileRead(char *argv,int num)
{
		//Structure Variable
		Read_t input;
		int cnt = 0;
		FILE *file;
		int i = 0 ;

		file = fopen(argv,"r");
		if(file == NULL)
		{
				printf("File Cannot open\n");
				exit(0);
		}
		else
		{
				//Reads the input pointed by the FILE *
				while(fscanf(file,"%d %lx",&input.mode,&input.address) != EOF)
				{

						if(input.mode == 8)
								statistics();
						//Calling Cases function to handle each mod
						Cases(input);


				}//while end
				printf("\n");

				//closing the file after reading
				fclose(file);

		}//else end

}//FileRead end


int init_cache()
{
		//Initialise all variables used to determine cache stats 
		No_of_Data_Reads = 0;
		No_of_Data_Writes = 0;
		No_of_Instruction_Reads = 0;
		No_of_Data_Hits = 0;
		No_of_Data_Misses = 0;
		No_of_Instruction_Hits = 0;
		No_of_Instruction_Misses = 0;
		Data_Hit_Ratio = 0;
		Instruction_Hit_Ratio = 0;

		//Dynamic Memory Allocation to Cache of 16K sets Both Instruction and Data cache
		Data_cache_block = (Block_bits_t **)malloc(LINES_IN_SET_DATA * SETS * sizeof(Block_bits_t*));
		Instruction_cache_block = (Block_bits_t **)malloc(LINES_IN_SET_DATA * SETS * sizeof(Block_bits_t*));

		if(Data_cache_block == NULL)
		{
				printf("Cache Memory Data is not allocated\n");
				return 1;
		}
		for(int i = 0;i < SETS; i++)
		{
				//Dynamic Memory Allocation to Each Cache set having "LINES_IN_SET_DATA" Lines in Data Cache 
				Data_cache_block[i] = (Block_bits_t*)malloc(LINES_IN_SET_DATA * sizeof(Block_bits_t));
				if(Data_cache_block[i] == NULL)
				{
						printf("Data Cache Set Memory is not allocated\n ");
						return 1;
				}
		}

		if(Instruction_cache_block == NULL)
		{
				printf("Cache Memory for Instruction is not allocated\n");
				return 1;
		}
		for(int i = 0;i< SETS; i++)
		{
				//Dynamic Memory Allocation to Each Cache set having "LINES_IN_SET_INSTRUCT" Lines in Instruction Cache 
				Instruction_cache_block[i] = (Block_bits_t*)malloc(LINES_IN_SET_INSTRUCT * sizeof(Block_bits_t));
				if(Instruction_cache_block[i] == NULL)
				{
						printf("Instruction Cache Set Memory is not allocated\n");
						return 1;
				}
		}

		for(int i = 0; i < SETS; i++)
		{
				for(int j = 0; j < LINES_IN_SET_DATA; j++)
				{
						Data_cache_block[i][j].tag = 0;
						Data_cache_block[i][j].lru = LINES_IN_SET_DATA - 1;
						Data_cache_block[i][j].mesi = INVALID;
				}

				for(int j = 0; j < LINES_IN_SET_INSTRUCT; j++)
				{
						Instruction_cache_block[i][j].tag = 0;
						Instruction_cache_block[i][j].lru = LINES_IN_SET_INSTRUCT - 1;
						Instruction_cache_block[i][j].mesi = INVALID;
				}
		}
}

void reset_cache()
{
		//Initialise all variables used to determine cache stats 
		No_of_Data_Reads = 0;
		No_of_Data_Writes = 0;
		No_of_Instruction_Reads = 0;
		No_of_Data_Hits = 0;
		No_of_Data_Misses = 0;
		No_of_Instruction_Hits = 0;
		No_of_Instruction_Misses = 0;
		Data_Hit_Ratio = 0;
		Instruction_Hit_Ratio = 0;
	

		for(int i = 0; i < SETS; i++)
		{
				for(int j = 0; j < LINES_IN_SET_DATA; j++)
				{
						if(Data_cache_block[i][j].mesi == MODIFIED)
						{
								unsigned long addr = reverse_address(i,j);
								if(num == 1)
										Communication(addr,WRITE_TO_L2);
								Data_cache_block[i][j].tag = 0;
								Data_cache_block[i][j].lru = LINES_IN_SET_DATA - 1;
								Data_cache_block[i][j].mesi = INVALID;
						}
						else
						{
						Data_cache_block[i][j].mesi = INVALID;
						Data_cache_block[i][j].tag = 0;
						Data_cache_block[i][j].lru = LINES_IN_SET_DATA - 1;
						}
				}
		}
}

unsigned long reverse_address(int index, int line)
{
	int rev_tag = (Data_cache_block[index][line].tag << 20);
	int rev_index = (index<<6);
	unsigned long addr = rev_tag + rev_index ; 
	return addr;
}

/*unsigned long mask(int n)
{
		unsigned long mask = ((0xFFFFFFFF << n) ^ (0xFFFFFFFF));
		return mask;
}*/

int get_tag(unsigned long addr)
{
		//Right shifting address bits 20 position to get Tag bits 
		//int tag_bits =	(addr >> 20);
		int tag_bits =	(addr >> (BYTE_OFFSET_SIZE + INDEX_SIZE));
		return tag_bits;
}

int get_index(unsigned long addr)
{
		//Right shifting address bits 6 position to get 14 bit index
		//int index_bits =	(addr>>6) & 0x3FFF;
		int index_bits =	(addr>> BYTE_OFFSET_SIZE) & (INDEX_MASK);
		return index_bits;
}

//Tp print the messages For L2 Cache
void Communication(unsigned long addr,int message)
{
		int tag = get_tag(addr);
		int index = get_index(addr);
		switch(message)
		{
				case RETURN_TO_L2:
						for(int line = 0; line  < LINES_IN_SET_DATA; line++)
						{
								if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi == MODIFIED))
								{
										if(num == 1)
												printf("\nReturn data to L2 %lx\n",addr);
										Data_cache_block[index][line].mesi = SHARED;
										break;
								}
								else if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi == EXCLUSIVE))
								{
										if(num == 1)
												printf("\nReturn data to L2 %lx\n",addr);
										Data_cache_block[index][line].mesi = SHARED;
										break;
								}
								else if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi == SHARED))
								{
										if(num == 1)
												printf("\nWARNING......\n");
										break;
								}

						}
						break;

				case WRITE_TO_L2:
						printf("\nWrite to L2 %lx\n",addr);
						break;

				case READ_FROM_L2:
						printf("\nRead from L2 %lx\n",addr);
						break;

				case READ_FOR_OWNERSHIP_L2:
						printf("\nRead for Ownership from L2 %lx\n",addr);
						break;
		}
}

void statistics()
{
		double Total_data = (double)No_of_Data_Misses+No_of_Data_Hits;
		double Total_instruction = (double)No_of_Instruction_Misses+No_of_Instruction_Hits;
		//Handling Arithmetic Exception for Data Cache Stats
		if( Total_data == 0)
		{
				Data_Hit_Ratio = 0.00;
		}
		//Displaying the Stats
		else
		{
				printf("/************************* STATISTICS *************************************************/\n");
				printf("/************************* DATA *******************************************************/\n");
				printf("Number of Data Reads: %d\t\tNumber of Data Writes: %d\n",No_of_Data_Reads,No_of_Data_Writes);
				printf("Number of Data Hits: %d\t\tNumber of Data Misses: %d\n",No_of_Data_Hits,No_of_Data_Misses);
				printf("\t\tData Hit Ratio: %lf\n",(double)No_of_Data_Hits/Total_data);
		}
		//Handling Arithmetic Exception for Instruction Cache Stats
		if(Total_instruction == 0)
		{
				Instruction_Hit_Ratio = 0.00;
		}
		//Displaying the Stats
		else
		{
				printf("/************************* INSTRUCTION ************************************************/\n");
				printf("Number of Instruction Reads: %d\n",No_of_Instruction_Reads);
				printf("Number of Instruction Hits: %d\t\tNumber of Instruction Misses: %d\n",No_of_Instruction_Hits,No_of_Instruction_Misses);
				printf("\t\tInstruction Hit Ratio: %lf\n",((double)No_of_Instruction_Hits/Total_instruction));
				printf("/****************************************************************************************/\n");
		}
}

