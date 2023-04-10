CC= gcc
Cflags= -ansi -Wall -Wextra -Werror -pedantic-errors -lm
# Cflags= -lm

# Specify the target executable and the source files needed to build it

spkmeans: spkmeans.o Utils.o spkmeanslogic.o spkmeans.h
	$(CC) -o spkmeans spkmeans.o spkmeanslogic.o Utils.o $(Cflags)
	
# spkmeans: spkmeans.o Utils.o spkmeanslogic.o hw2kmeans.o spkmeans.h
# 	$(CC) -o spkmeans spkmeans.o spkmeanslogic.o Utils.o hw2kmeans.o $(Cflags)
# Specify the object files that are generated from the corresponding source files
spkmeans.o: spkmeans.c
	$(CC) -c spkmeans.c $(Cflags)
spkmeanslogic.o: spkmeanslogic.c
	$(CC) -c spkmeanslogic.c $(Cflags)
Utils.o: Utils.c
	$(CC) -c Utils.c $(Cflags)
# kmeans.o: hw2kmeans.c
# 	$(CC) -c hw2kmeans.c $(Cflags)
