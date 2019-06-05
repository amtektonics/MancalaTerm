CC=gcc
LDFLAGS=-lncurses
TARGET=mancala

all: $(TARGET).c
	$(CC) $(TARGET).c $(LDFLAGS) -o $(TARGET)


clean: $(TARGET)
	\rm $(TARGET)


