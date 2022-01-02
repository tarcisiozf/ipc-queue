CC=gcc
CFLAGS=-Wall -g

all: producer consumer

shared_mem.o: shared_mem.h
	$(CC) $(CFLAGS) -c shared_mem.c

shared_queue.o: shared_queue.h shared_mem.o
	$(CC) $(CFLAGS) -c shared_queue.c

producer: producer.c shared_mem.o shared_queue.o
	$(CC) $(CFLAGS) -o $@ $^

consumer: consumer.c shared_mem.o shared_queue.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *.bin producer consumer