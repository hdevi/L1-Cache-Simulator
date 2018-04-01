#ifndef __MAIN_H
#define __MAIN_H

//Standard C Header Files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//Cache Specifications
#define SETS (16*1024)
#define LINES_IN_SET_INSTRUCT 2
#define LINES_IN_SET_DATA 4

#define BYTE_DATA_LINE 64
#define BYTE_OFFSET_SIZE (int)(log(BYTE_DATA_LINE)/log(2))
#define INDEX_SIZE (int)(log(SETS)/log(2))
#define INDEX_MASK (int)((1<<INDEX_SIZE)-1)

#define BIT_PROCESSOR 32
#define TAG_SIZE (BIT_PROCESSOR - INDEX_SIZE - BYTE_OFFSET_SIZE)

//MESI Protocol Constants
#define MODIFIED 0
#define EXCLUSIVE 1
#define SHARED 2
#define INVALID 3

#define HIT 1
#define MISS 0
#define WRITE 1
#define READ 0

#define RETURN_TO_L2 1
#define WRITE_TO_L2 2
#define READ_FROM_L2 3
#define READ_FOR_OWNERSHIP_L2 4

//#define DEBUG2 1

//Structure Declarations
typedef struct Read
{
		int mode;
		unsigned long address; 
}Read_t;


typedef struct Block
{
		int mesi;
		int tag;
		int lru;
}Block_bits_t;

//Variable Declaration
int num;
int	No_of_Data_Reads;
int	No_of_Data_Writes;
int	No_of_Instruction_Reads;
int	No_of_Data_Hits;
int	No_of_Data_Misses;
int	No_of_Instruction_Hits;
int	No_of_Instruction_Misses;
double Data_Hit_Ratio;
double Instruction_Hit_Ratio;

//Function Declarations
void FileRead(char *,int);
void Cases(Read_t);
int init_cache();
void reset_cache();
int get_tag(unsigned long);
int get_index(unsigned long);
unsigned long reverse_address(int index, int line);

int check_Data_cache(unsigned long);
int check_Instruction_cache(unsigned long);

int lru_update_Data(unsigned long addr);
int lru_update_Instruction(unsigned long addr);

int evict_Data(unsigned long addr);
int evict_Instruction(unsigned long addr);

void mesi_update_Data(unsigned long addr, int operation,int status,int line);
void mesi_update_Instruction(unsigned long addr, int operation,int status,int line);

int check_Data_cache_for_Invalidation(unsigned long addr);
void Communication(unsigned long addr,int message);

void print_Data_info(int mode,unsigned long address,int index,int status);
void print_Instruction_info(int mode,unsigned long address,int index,int status);
void Data_contents(int way,int index);
void Instruction_contents(int way,int index);
void statistics();

extern Block_bits_t **Data_cache_block;
extern Block_bits_t **Instruction_cache_block;
#endif
