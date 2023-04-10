CC = g++
CFLAGS = -Wall
OBJECTS = frontEnd

all: $(OBJECTS)

$(OBJECTS): %: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean purge

purge:
	ipcrm -a

clean:
	$(RM) $(OBJECTS) *.o
