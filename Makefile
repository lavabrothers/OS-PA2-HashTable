CC = gcc
CFLAGS = -Wall -Wextra -pthread

SRCS = chash.c hash_table.c parser.c
OBJS = $(SRCS:.c=.o)
TARGET = chash

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)