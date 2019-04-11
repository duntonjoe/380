CC = gcc
CFLAGS = -O2 -MMD

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

ifdef NDEBUG
CFLAGS += -DNDEBUG
endif

ifdef SLEEP_FACTOR
CFLAGS += -DSLEEP_FACTOR=$(SLEEP_FACTOR)
endif

ifdef ARRIVE_FACTOR
CFLAGS += -DARRIVE_FACTOR=$(ARRIVE_FACTOR)
endif

ifdef EAT_FACTOR
CFLAGS += -DEAT_FACTOR=$(EAT_FACTOR)
endif

.PHONY : clean tidy handin

dining : $(OBJS)
	$(LINK.o) $^ -o $@ -pthread

clean :
	-rm -vf $(OBJS)

tidy:
	-rm -vf $(OBJS) dining $(DEPS)

handin: handin.tar

handin.tar : table.c table.h WRITEUP.txt
	tar cvf $@ $^

WRITEUP.txt :
	touch $@

-include $(DEPS)
