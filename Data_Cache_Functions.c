#include"main.h"

Block_bits_t **Data_cache_block;
Block_bits_t **Instruction_cache_block;

/*check_Data_cache function checks the status of cache line if its HIT or MISS*/
int check_Data_cache(unsigned long addr)
{
		int tag = get_tag(addr);
		int line,flag = 0;
		int index = get_index(addr);
		for( line = 0;line < LINES_IN_SET_DATA; line++)
		{
				if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi != INVALID))
				{
						flag = 1;
				}
		}
		if(flag == 1)
				return HIT;
		else
				return MISS;
}

/* This function is invoked when other processor sends an Invalidation signal on Bus*/
int check_Data_cache_for_Invalidation(unsigned long addr)
{

		int tag = 0,index = 0,line;
		tag = get_tag(addr);
		index = get_index(addr);
		for( line = 0;line < LINES_IN_SET_DATA; line++)
		{
				if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi != INVALID))
				{

						if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi == SHARED))
						{
								Data_cache_block[index][line].lru = LINES_IN_SET_DATA - 1;
								Data_cache_block[index][line].mesi = INVALID;
								break;
						}
						else if((Data_cache_block[index][line].tag == tag) && (Data_cache_block[index][line].mesi == MODIFIED) | (Data_cache_block[index][line].mesi == EXCLUSIVE))
						{
								if(num == 1)
										printf("\nWARNING....\n");
								break;
						}
				}
		}
		return line;
}

/*lru_update function updates the lru bits for the lines in Cache using 
	Least Recently Used policy by Counter implementation*/
int lru_update_Data(unsigned long addr)
{
		int index = get_index(addr);
		int	tag = get_tag(addr);
		int line;
		for(line = 0; line < LINES_IN_SET_DATA; line++)
		{
				if(Data_cache_block[index][line].tag == tag) //its hit
				{
						/*The line which needs to be evicted has some lru bit assign 
							currently that lru bit are stored in variable lru_bits below*/
						int lru_bits = Data_cache_block[index][line].lru; 

						for(int line1 = 0; line1 < LINES_IN_SET_DATA; line1++)
						{
								//Checks if LRU bits are less than the one evicted
								if(Data_cache_block[index][line1].lru < lru_bits)
								{
										Data_cache_block[index][line1].lru++;
								}

						}
						//Assigns current line lru bit to 0
						Data_cache_block[index][line].lru = 0;
						break;
				}

		}
		return line;
}

/* evit_Data function checks first that the line in a set in INVALID or not.
	 If it's INVALID then it will evict the INVALID line first. Else if there 
	 is no INVALID line then it would evict least recently used line in a set.
	 Also in both Cases it updates the lru bits of lines calling lru_update function.*/
int evict_Data(unsigned long addr)
{
		int index = get_index(addr);
		int tag = get_tag(addr);
		int lru_line,line;
		for(int line1 = 0; line1 < LINES_IN_SET_DATA; line1++)
		{
				if(Data_cache_block[index][line1].mesi == INVALID)
				{
						Data_cache_block[index][line1].tag = tag;
						lru_line = lru_update_Data(addr);
						return lru_line;
						break;
				}
		}

		for(line = 0; line < LINES_IN_SET_DATA; line++)
		{
				if((Data_cache_block[index][line].lru == LINES_IN_SET_DATA - 1) && (Data_cache_block[index][line].mesi != INVALID))
				{
						if(Data_cache_block[index][line].mesi == MODIFIED)
						{
								unsigned long addr = reverse_address(index,line);
								if(num == 1)
										Communication(addr,WRITE_TO_L2);
						}

						Data_cache_block[index][line].tag = tag;
						lru_line = lru_update_Data(addr);
						return lru_line;
						break;
				}
		}
}

/* Mesi update function to make the state of a line in set as per 
	 MESI protocol to avoid Cache Coherence Problem.
	 operation is READ/WRITE or something
	 Status would be HIT/MISS*/

void mesi_update_Data(unsigned long addr, int operation,int status,int line)
{
		int tag = get_tag(addr);
		int index = get_index(addr);


		//update state when HIT
		if(status == HIT)
		{
				if((operation == WRITE) && (Data_cache_block[index][line].mesi == SHARED))
				{
						Data_cache_block[index][line].mesi = EXCLUSIVE;
						if(num == 1)
								Communication(addr,WRITE_TO_L2);
				}
				else if(operation == WRITE)
						Data_cache_block[index][line].mesi = MODIFIED;
		}
		//update state when MISS
		else
		{
				//For first Write
				if(operation == WRITE)
				{
						Data_cache_block[index][line].mesi = EXCLUSIVE;
						if(num == 1)
								Communication(addr,WRITE_TO_L2);
				}
				//For Read
				else
						Data_cache_block[index][line].mesi = SHARED;
		}
}

//Displaying the results of each operation by calling print_Data_info function and Data_contents function
void print_Data_info(int mode,unsigned long address,int index,int status)
{
		char *string = NULL;
		if(status == 1)
				string = "HIT";
		else if(status == 0)
				string = "MISS";
		else
				string = " ";

		printf("\n*****************************INFORMATION OF DATA CACHE************************************");
		printf("\n*********************************************************************************************************");
		printf("\n	MODE: %d			ADDRESS: %lx   		INDEX: %x			STATUS: %s\n",mode,address,index,string);
		printf("\n*********************************************************************************************************");
}
void Data_contents(int way,int index)
{
		printf("WAY:%d TAG:%x LRU:%d MESI:%s | ",way,Data_cache_block[index][way].tag,Data_cache_block[index][way].lru,((Data_cache_block[index][way].mesi==MODIFIED)?"M":(Data_cache_block[index][way].mesi==EXCLUSIVE)?"E":(Data_cache_block[index][way].mesi==SHARED)?"S":"I"));
}
