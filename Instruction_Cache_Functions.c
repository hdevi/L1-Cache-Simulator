#include"main.h"

Block_bits_t **Data_cache_block;
Block_bits_t **Instruction_cache_block;

/*check_Instruction_cache function checks the status of cache line if its HIT or MISS*/
int check_Instruction_cache(unsigned long addr)
{
		int tag = get_tag(addr);
		int line,flag = 0;
		int index = get_index(addr);
		for( line = 0;line < LINES_IN_SET_INSTRUCT; line++)
		{

				if((Instruction_cache_block[index][line].tag == tag) && (Instruction_cache_block[index][line].mesi != INVALID))
				{
						flag = 1;
				}
		}
		if(flag == 1)
				return HIT;
		else
				return MISS;
}

/*lru_update function updates the lru bits for the lines in Cache using 
	Least Recently Used policy by Counter implementation*/

int lru_update_Instruction(unsigned long addr)
{
		int index = get_index(addr);
		int	tag = get_tag(addr);
		int line;
		for(line = 0; line < LINES_IN_SET_INSTRUCT; line++)
		{
				if(Instruction_cache_block[index][line].tag == tag)
				{
						int lru_bits = Instruction_cache_block[index][line].lru;

						for(int line1 = 0; line1 < LINES_IN_SET_INSTRUCT; line1++)
						{
								if(Instruction_cache_block[index][line1].lru < lru_bits)
								{
										Instruction_cache_block[index][line1].lru++;
								}

						}
						Instruction_cache_block[index][line].lru = 0;
						break;
				}
		}
		return line;
}
/* evit_Data function checks first that the line in a set in INVALID or not.
	 If it's INVALID then it will evict the INVALID line first. Else if there 
	 is no INVALID line then it would evict least recently used line in a set.
	 Also in both Cases it updates the lru bits of lines calling lru_update function.*/

int evict_Instruction(unsigned long addr)
{
		int index = get_index(addr);
		int tag = get_tag(addr);
		int lru_line,line;
		for(int line1 = 0; line1 < LINES_IN_SET_INSTRUCT; line1++)
		{
				if(Instruction_cache_block[index][line1].mesi == INVALID)
				{
						Instruction_cache_block[index][line1].tag = tag;
						lru_line = lru_update_Instruction(addr);
						return lru_line;
						break;
				}
		}
		for(line = 0; line < LINES_IN_SET_INSTRUCT; line++)
		{
				if(Instruction_cache_block[index][line].lru == LINES_IN_SET_INSTRUCT - 1)
				{
						if(Instruction_cache_block[index][line].mesi == SHARED)
						{
								Instruction_cache_block[index][line].tag = tag;
								lru_line = lru_update_Instruction(addr);
						}
						return lru_line;
						break;
				}
		}
}

/* Mesi update function to make the state of a line in set as per 
	 MESI protocol to avoid Cache Coherence Problem.
	 operation is READ/WRITE or something
	 Status would be HIT/MISS*/
void mesi_update_Instruction(unsigned long addr, int operation,int status,int line)
{
		int tag = get_tag(addr);
		int index = get_index(addr);

		if(status == HIT)
		{
				Instruction_cache_block[index][line].mesi = SHARED;
		}
		else
				Instruction_cache_block[index][line].mesi = SHARED;

}

//Displaying the results of each operation by calling print_Data_info function and Data_contents function
void print_Instruction_info(int mode,unsigned long address,int index,int status)
{
		char *string = NULL;
		if(status == 1)
				string = "HIT";
		else if(status == 0)
				string = "MISS";
		else
				string = " ";

		printf("\n*****************************INFORMATION OF INSTRUCTION CACHE************************************");
		printf("\n*********************************************************************************************************");
		printf("\n	MODE: %d			ADDRESS: %lx   		INDEX: %x			STATUS: %s\n",mode,address,index,string);
		printf("\n*********************************************************************************************************");
}

/*void Instruction_contents(int way,int index)
{
		printf("\n	INDEX: %x			TAG: %x	  		WAY: %d\n",index,Instruction_cache_block[index][way].tag,way);
		printf("\n	LRU: %d   		MESI_STATE: %s\n",Instruction_cache_block[index][way].lru,((Instruction_cache_block[index][way].mesi==MODIFIED)?"MODIFIED":(Instruction_cache_block[index][way].mesi==EXCLUSIVE)?"EXCLUSIVE":(Instruction_cache_block[index][way].mesi==SHARED)?"SHARED":"INVALID"));
		printf("--------------------------------------------------------------------------------------------------------------\n");
}*/
void Instruction_contents(int way,int index)
{
		printf("WAY:%d TAG:%x LRU:%d MESI:%s | ",way,Instruction_cache_block[index][way].tag,Instruction_cache_block[index][way].lru,((Instruction_cache_block[index][way].mesi==MODIFIED)?"M":(Instruction_cache_block[index][way].mesi==EXCLUSIVE)?"E":(Instruction_cache_block[index][way].mesi==SHARED)?"S":"I"));
}
