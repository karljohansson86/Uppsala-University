DEBUG=y

CC=gcc
CFLAGS=-std=gnu99 -Wall 
LDFLAGS= 

ifeq ($DEBUG), y)
	CFLAGS += -DDEBUG -g
else
	CFLAGS += -O2
endif


all: bin/shell bin/parser_test_interactive bin/parser_test_auto

bin/shell: obj/shell.o obj/parser.o obj/debug.o
	gcc $^ -o $@

bin/parser_test%: obj/parser.o obj/parser_test%.o
	gcc $^ -o $@

obj/%.o: src/%.c
	gcc -c $(CFLAGS) $< -o $@

clean:
	$(RM) *~ src/*~ src/#* obj/*.o bin/* 

# 	
.PHONY: all clean dist

