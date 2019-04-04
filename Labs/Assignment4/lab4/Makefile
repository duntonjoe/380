CC = gcc
CPPFLAGS = -MMD
CFLAGS = -O2 -Wall -Wextra
LDLIBS = -pthread

DIFFICULTY ?= 1
CPPFLAGS += -DDIFFICULTY=${DIFFICULTY}

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

.PHONY: all clean tidy handin

all : driver

driver : $(OBJS)
	$(LINK.o) $^ $(LDLIBS) -o $@

clean :
	-rm -vf $(OBJS) driver

tidy :
	-rm -vf $(DEPS) $(OBJS) driver

handin : handin.tar

handin.tar : WRITEUP.txt semaphore.c queue.h queue.c
	@tar cvf $@ $^
	@echo "Submit 'handin.tar' to autolab"

-include ${DEPS}
