CC = g++
CFLAGS = -Wall
OBJECTS = P1

all: $(OBJECTS)

$(OBJECTS): %: %.cc
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean purge

purge:
	ipcrm -a

clean:
	$(RM) $(OBJECTS) *.o
