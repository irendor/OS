CC = gcc
FLAGS = -c -Wall -Werror -Wextra 

all: archive.o 
	$(CC) *.o -o ./a.out 

archive.o: archive.c 
	$(CC) $(FLAGS) archive.c 

clean:
	rm -rf *.o ./a.out archive folder_unarch/* folder_unarch