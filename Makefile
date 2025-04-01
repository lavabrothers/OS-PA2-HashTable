CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJS = chash.o hash_table.o rw_lock.o command_processor.o

all: chash

chash: $(OBJS)
	$(CC) $(CFLAGS) -o chash $(OBJS)

chash.o: chash.c hash_table.h rw_lock.h command_processor.h
	$(CC) $(CFLAGS) -c chash.c

hash_table.o: hash_table.c hash_table.h rw_lock.h
	$(CC) $(CFLAGS) -c hash_table.c

rw_lock.o: rw_lock.c rw_lock.h
	$(CC) $(CFLAGS) -c rw_lock.c

command_processor.o: command_processor.c command_processor.h hash_table.h
	$(CC) $(CFLAGS) -c command_processor.c

clean:
	rm -f chash $(OBJS) output.txt