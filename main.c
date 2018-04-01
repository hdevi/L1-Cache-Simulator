#include "main.h"

int main(int argc, char *argv[])
{
		system("clear");

		//Initializing Cache
		init_cache();

		char *argv2 = argv[2]; 
		num = atoi(argv2);

		//Reading from .txt file
		FileRead(argv[1],num);

		printf("\n");
		//Calling function to Display Cache Statistics

		statistics();
		return 0;
}
