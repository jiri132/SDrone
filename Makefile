CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iunity -I.
OBJS = main.o Communication.o hamming.o protocolLayer.o
TEST_OBJS = test/test_communication.o test/test_protocollayer.o \
            Communication.o protocolLayer.o hamming.o

test_communication: test/test_communication.o Communication.o protocolLayer.o hamming.o unity/unity.o
	$(CC) $(CFLAGS) -o test_communication $^

test_protocollayer: test/test_protocollayer.o Communication.o protocolLayer.o hamming.o unity/unity.o
	$(CC) $(CFLAGS) -o test_protocollayer $^

tests: test_communication test_protocollayer
	./test_communication
	./test_protocollayer

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS)

main.o: main.c Communication.h hamming.h protocolLayer.h
	$(CC) $(CFLAGS) -c main.c

Communication.o: Communication.c Communication.h
	$(CC) $(CFLAGS) -c Communication.c

hamming.o: hamming.c hamming.h hamming_types.h
	$(CC) $(CFLAGS) -c hamming.c

protocolLayer.o: protocolLayer.c protocolLayer.h
	$(CC) $(CFLAGS) -c protocolLayer.c

unity/unity.o: unity/unity.c unity/unity.h
	$(CC) $(CFLAGS) -c unity/unity.c -o unity/unity.o

clean:
	rm -f *.o test/*.o main tests unity/unity.o
