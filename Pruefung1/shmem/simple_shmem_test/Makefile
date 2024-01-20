
DEFS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
override CFLAGS += -Wall -g -std=c99 -pedantic $(DEFS)
override LDFLAGS +=
override LIBS += -lrt -lpthread

# rules

.PHONY : all clean

all: writer supervisor

writer: writer.o
	gcc $(LDFLAGS) -o $@ $^ $(LIBS)

supervisor: supervisor.o
	gcc $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f writer supervisor *.o