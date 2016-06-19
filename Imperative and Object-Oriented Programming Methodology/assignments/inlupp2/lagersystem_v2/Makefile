all: lagersystem

CC	= gcc
FLAGS	= -Wall -ggdb -std=c11 -D _GNU_SOURCE

lagersystem: lagersystem.o storage.o history.o ui.o controller.o btree.o product.o cart.o list.o queue.o input.o
	$(CC) $(FLAGS) $^ -o lagersystem

lagersystem.o: lagersystem.c lagersystem.h
	$(CC) $(FLAGS) -c lagersystem.c

product.o: product.c product.h
	$(CC) $(FLAGS) -c $<

storage.o: storage.c storage.h btree.o list.o product.o history.o input.o
	$(CC) $(FLAGS) -c storage.c

history.o: history.c history.h queue.o
	$(CC) $(FLAGS) -c history.c

list.o: list.c list.h
	$(CC) $(FLAGS) -c list.c

btree.o: btree.c btree.h
	$(CC) $(FLAGS) -c btree.c

cart.o: cart.c cart.h
	$(CC) $(FLAGS) -c cart.c

ui.o: ui.c ui.h storage.o
	$(CC) $(FLAGS) -c ui.c

queue.o: queue.c queue.h list.o
	$(CC) $(FLAGS) -c queue.c

controller.o: controller.c controller.h storage.o history.o ui.o
	$(CC) $(FLAGS) -c controller.c

input.o: input.c input.h
	$(CC) $(FLAGS) -c input.c

dox:
	@rm -rf docs
	@doxygen doxygen_config

clean:
	@echo "\n\tI feel a strange force hovering in this folder..."
	@rm -f *.out
	@rm -f *.o
	@rm -f lagersystem
	@rm -rf *.dSYM
	@rm -f *~
	@rm -f *.gch
	@rm -f ./tests/*~
	@rm -rf tests/compiled/*
	@echo "\tThis house has been cleansed!\n"

# TEST CASES
# Compile all tests in test folder and run them
test: 
	@ls ./tests | grep test_.*\.c$$ | xargs make

test_storage.c: tests/test_storage.c storage.o  btree.o list.o product.o
	$(CC) $(FLAGS)  $^ -o tests/compiled/test_storage -lcunit
	@./tests/compiled/test_storage

# Single test compilation (catch-all)
test_%.c: tests/test_%.c %.o
	$(CC) $(FLAGS)  $^ -o tests/compiled/$@ -lcunit
	@valgrind ./tests/compiled/$@
