# The name of the program
PROG_NAME := writer

# Picks all the .c files in this directory
C_FILES := $(wildcard *.c)
OBJS := $(C_FILES:.c=.o)

# Cross compilation option
CC := $(CROSS_COMPILE)gcc

$(PROG_NAME): $(OBJS)
	$(CC) $^ -o $@

%.o : %.c
	$(CC) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(PROG_NAME)
