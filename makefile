CC = g++
CFLAGS = -Wall
NAME = frontEnd
OBJECTS = frontEnd.o parser.o scanner.o staticsem.o

all: $(NAME) tidy

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

.PHONY: clean purge tidy

purge:
	ipcrm -a

clean:
	rm -f $(OBJECTS).o $(NAME)

tidy:
	rm -f $(OBJECTS)