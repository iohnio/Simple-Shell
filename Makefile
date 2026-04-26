CC = gcc
CFLAGS = -g -Wall -Werror
TARGET = shell

build: shell.o utils.o
	$(CC) $(CFLAGS) shell.o utils.o -o $(TARGET)
shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
utils.o: utils.c
	$(CC) $(CFLAGS) -Wno-unused-variable -c utils.c
clean:
	rm -f $(TARGET) *.o .421history
