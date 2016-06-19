#
# Makefile for problem set 2, introduction to synchronizaiton.
#
# Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
#

# Change to y to enable debugging support
DEBUG:=


CFLAGS=-pthread -std=c99 -D_XOPEN_SOURCE=600 -Wall -Wextra
LDLIBS=-pthread -lrt

CC=gcc

ifeq ($(DEBUG), y)
	CFLAGS += -g
	LDFLAGS += -g
else
	CFLAGS += -O2
	LDFLAGS += -O2
endif

all: bounded_buffer rendezvous synchro

bounded_buffer: bounded_buffer.o
rendezvous: rendezvous.o
synchro: synchro.o timing.o

%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o
	@gcc -MM $(CFLAGS) $*.c > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

-include $(CFILES:%.c=%.d)

clean:
	$(RM) *.o *.d *~ $(EXEFILES)

distclean: clean
	$(RM) synchro rendezvous bounded_buffer

.PHONY: all clean
