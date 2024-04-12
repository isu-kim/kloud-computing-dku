.PHONY: all clean

CC = gcc
CFLAGS = -O2 -Wall -std=gnu99
LDFLAGS =

PROG = engineX

SRCDIRS := $(shell find . -type d)
SRC := $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
OBJ := $(SRC:.c=.o)

all:
	make $(PROG)
build: 
	make $(PROG)

debug: CFLAGS += -DDEBUG -g
debug: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(PROG) $(OBJ)

