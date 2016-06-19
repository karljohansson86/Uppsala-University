

CC = gcc
FLAGS = -Wall -std=c11 -ggdb


all: warehousesystem

#Bulid warehousesystem
warehousesystem: undo.o warehousesystem.c 
	$(CC) $(FLAGS) -o $@ $^



#hjälpfiler..moduler
%.o: %.c %.h
	$(CC) $(FLAGS) -c $^

run: warehousesystem
	./warehousesystem

clean:
	rm -f warehousesystem
	rm -f *.o
	rm -f x-r *.dSYM
	rm -f -r bin/*
