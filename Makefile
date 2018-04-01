main.out: main.o Functions.o Data_Cache_Functions.o Instruction_Cache_Functions.o Cases.o
					gcc -o main.out main.o Functions.o Data_Cache_Functions.o Instruction_Cache_Functions.o Cases.o

main.o: main.c main.h
				gcc -c main.c

Functions.o: Functions.c main.h
				gcc -c Functions.c

Data_Cache_Functions.o: Data_Cache_Functions.c main.h
				gcc -c Data_Cache_Functions.c

Instruction_Cache_Functions.o:Instruction_Cache_Functions.c main.h
				gcc -c Instruction_Cache_Functions.c

Cases.o: Cases.c main.h
				gcc -c Cases.c

run:
				./main.out ${ARGS}

clean:
				rm -r *.o
				rm main.out
