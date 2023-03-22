CC= gcc
Cflags= -ansi -Wall -Wextra -Werror -pedantic-errors -lm
# Specify the target executable and the source files needed to build it
main: spkmeans.o spkmeanslogic.o Utils.o kmeans.o spkmeans.h
	$(CC) -o main spkmeans.o spkmeanslogic.o Utils.o kmeans.o $(Cflags)
# Specify the object files that are generated from the corresponding source files
spkmeans.o: spkmeans.c
	$(CC) -c spkmeans.c $(Cflags)
spkmeanslogic.o: spkmeanslogic.c
	$(CC) -c spkmeanslogic.c $(Cflags)
Utils.o: Utils.c
	$(CC) -c Utils.c $(Cflags)
kmeans.o: kmeans.c
	$(CC) -c kmeans.c $(Cflags)
	