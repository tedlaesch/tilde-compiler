CC = g++
CFLAGS = -Wall
NAME = genCode
OBJECTS = frontEnd.o parser.o scanner.o staticsem.o asmgen.o

all: $(NAME) tidy

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

.PHONY: clean purge tidy

purge:
	ipcrm -a

clean:
	rm -f *.asm $(OBJECTS).o $(NAME)

tidy:
	rm -f $(OBJECTS)