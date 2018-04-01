#include"main.h"

void Cases(Read_t input)
{
		switch(input.mode)
		{
				case 0:
						No_of_Data_Reads++;
						//It calls the check_Data_Cache function to check it's READ(MISS or HIT)
						int L1_check_R = check_Data_cache(input.address);
						if(L1_check_R == HIT)
						{
								No_of_Data_Hits++;
								int index = get_index(input.address);
								int lru_line = lru_update_Data(input.address);
								mesi_update_Data(input.address,READ,L1_check_R,lru_line);
						}
						//MISS block
						else
						{
								No_of_Data_Misses++;
								int index = get_index(input.address);
								if(num == 1)
										Communication(input.address,READ_FROM_L2);
								int evicted_line = evict_Data(input.address);
								mesi_update_Data(input.address,READ,L1_check_R,evicted_line);
						}
						break;

				case 1:
						No_of_Data_Writes++;
						//It calls the check_Data_Cache function to check it's WRITE(MISS or HIT)
						int L1_check_W = check_Data_cache(input.address);
						if(L1_check_W == HIT)
						{
								No_of_Data_Hits++;
								int index = get_index(input.address);
								int lru_line = lru_update_Data(input.address);
								mesi_update_Data(input.address,WRITE,L1_check_W,lru_line);
						}
						//MISS Block
						else
						{
								No_of_Data_Misses++;
								int index = get_index(input.address);
								int evicted_line = evict_Data(input.address);
								if(num == 1)
										Communication(input.address,READ_FOR_OWNERSHIP_L2);
								mesi_update_Data(input.address,WRITE,L1_check_W,evicted_line);
						}
						break;

				case 2:
						No_of_Instruction_Reads++;
						int L1_check_IR = check_Instruction_cache(input.address);
						if(L1_check_IR == HIT)
						{
								No_of_Instruction_Hits++;
								int lru_line = lru_update_Instruction(input.address);
								int index = get_index(input.address);
								mesi_update_Instruction(input.address,READ,L1_check_IR,lru_line);
						}
						else
						{
								No_of_Instruction_Misses++;
								int index = get_index(input.address);
								if(num == 1)
										Communication(input.address,READ_FROM_L2);
								int evicted_line = evict_Instruction(input.address);
								mesi_update_Instruction(input.address,READ,L1_check_IR,evicted_line);
						}
						break;

				case 3:
						{

								int index = get_index(input.address);
								int invalid_line = check_Data_cache_for_Invalidation(input.address);
						}
						break;

				case 4:
						Communication(input.address,RETURN_TO_L2);	
						break;

				case 8:	
						reset_cache();								
						break;

				case 9:	
						{
								int index = get_index(input.address);
								int	tag = get_tag(input.address);
								int line,set;
								int i = 1;
								printf("\nDATA CACHE\n");
								for(set = 0 ; set < SETS ; set++)
								{
										for(line = 0; line < LINES_IN_SET_DATA; line++)
										{
												
												if(Data_cache_block[set][line].mesi != INVALID)
												{
														if(i == 1)
														{
																printf("\n");
																printf("SET:%x  ",set);
														}
														Data_contents(line,set);
														i++;
												}
												

										}
										i = 1;

								}
							

								i = 1;
								printf("\n");
								printf("\nINSTRUCTION CACHE\n");
								for(set = 0 ; set < SETS ; set++)
								{

										for(line = 0; line < LINES_IN_SET_INSTRUCT; line++)

										{
												if(Instruction_cache_block[set][line].mesi != INVALID)
												{
														if(i == 1)
														{
																printf("\n");
																printf("SET:%x  ",set);
														}
														Instruction_contents(line,set);
														i++;
												}
										}
												 i = 1;
								}
								printf("\n");
								
						}
						printf("--------------------------------------------------------\n");
						break;


				default:
						printf("%d Invalid Mode\n",input.mode);
						break;

		}
}
