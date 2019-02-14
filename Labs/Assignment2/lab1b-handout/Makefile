# Makefile for the CS:APP Shell Lab

DRIVER = ./sdriver.pl
TSH = ./tsh
TSHREF = /home/faculty/wkillian/Public/380/tshref

CC = gcc
CFLAGS = -Wall -O2 -g

FILES = $(TSH) ./myspin ./mysplit ./mystop ./myint

.PHONY: all test rtest clean

all: $(FILES)

tsh: tsh.o jobctrl.o
	$(LINK.c) -o $@ $^

##################
# Regression tests
##################

test: test01 test02 test03 test04 test05 test06 test07 test08 test09 test10 test11 test12 test13 test14 test15 test16 test17 test18

rtest: rtest01 rtest02 rtest03 rtest04 rtest05 rtest06 rtest07 rtest08 rtest09 rtest10 rtest11 rtest12 rtest13 rtest14 rtest15 rtest16 rtest17 rtest18

# Run tests using the student's shell program
test01:
	$(DRIVER) -t ./traces/trace01.txt -s $(TSH) -a $(TSHARGS)
test02:
	$(DRIVER) -t ./traces/trace02.txt -s $(TSH) -a $(TSHARGS)
test03:
	$(DRIVER) -t ./traces/trace03.txt -s $(TSH) -a $(TSHARGS)
test04:
	$(DRIVER) -t ./traces/trace04.txt -s $(TSH) -a $(TSHARGS)
test05:
	$(DRIVER) -t ./traces/trace05.txt -s $(TSH) -a $(TSHARGS)
test06:
	$(DRIVER) -t ./traces/trace06.txt -s $(TSH) -a $(TSHARGS)
test07:
	$(DRIVER) -t ./traces/trace07.txt -s $(TSH) -a $(TSHARGS)
test08:
	$(DRIVER) -t ./traces/trace08.txt -s $(TSH) -a $(TSHARGS)
test09:
	$(DRIVER) -t ./traces/trace09.txt -s $(TSH) -a $(TSHARGS)
test10:
	$(DRIVER) -t ./traces/trace10.txt -s $(TSH) -a $(TSHARGS)
test11:
	$(DRIVER) -t ./traces/trace11.txt -s $(TSH) -a $(TSHARGS)
test12:
	$(DRIVER) -t ./traces/trace12.txt -s $(TSH) -a $(TSHARGS)
test13:
	$(DRIVER) -t ./traces/trace13.txt -s $(TSH) -a $(TSHARGS)
test14:
	$(DRIVER) -t ./traces/trace14.txt -s $(TSH) -a $(TSHARGS)
test15:
	$(DRIVER) -t ./traces/trace15.txt -s $(TSH) -a $(TSHARGS)
test16:
	$(DRIVER) -t ./traces/trace16.txt -s $(TSH) -a $(TSHARGS)
test17:
	$(DRIVER) -t ./traces/trace17.txt -s $(TSH) -a $(TSHARGS)
test18:
	$(DRIVER) -t ./traces/trace18.txt -s $(TSH) -a $(TSHARGS)


# Run the tests using the reference shell program
rtest01:
	$(DRIVER) -t ./traces/trace01.txt -s $(TSHREF) -a $(TSHARGS)
rtest02:
	$(DRIVER) -t ./traces/trace02.txt -s $(TSHREF) -a $(TSHARGS)
rtest03:
	$(DRIVER) -t ./traces/trace03.txt -s $(TSHREF) -a $(TSHARGS)
rtest04:
	$(DRIVER) -t ./traces/trace04.txt -s $(TSHREF) -a $(TSHARGS)
rtest05:
	$(DRIVER) -t ./traces/trace05.txt -s $(TSHREF) -a $(TSHARGS)
rtest06:
	$(DRIVER) -t ./traces/trace06.txt -s $(TSHREF) -a $(TSHARGS)
rtest07:
	$(DRIVER) -t ./traces/trace07.txt -s $(TSHREF) -a $(TSHARGS)
rtest08:
	$(DRIVER) -t ./traces/trace08.txt -s $(TSHREF) -a $(TSHARGS)
rtest09:
	$(DRIVER) -t ./traces/trace09.txt -s $(TSHREF) -a $(TSHARGS)
rtest10:
	$(DRIVER) -t ./traces/trace10.txt -s $(TSHREF) -a $(TSHARGS)
rtest11:
	$(DRIVER) -t ./traces/trace11.txt -s $(TSHREF) -a $(TSHARGS)
rtest12:
	$(DRIVER) -t ./traces/trace12.txt -s $(TSHREF) -a $(TSHARGS)
rtest13:
	$(DRIVER) -t ./traces/trace13.txt -s $(TSHREF) -a $(TSHARGS)
rtest14:
	$(DRIVER) -t ./traces/trace14.txt -s $(TSHREF) -a $(TSHARGS)
rtest15:
	$(DRIVER) -t ./traces/trace15.txt -s $(TSHREF) -a $(TSHARGS)
rtest16:
	$(DRIVER) -t ./traces/trace16.txt -s $(TSHREF) -a $(TSHARGS)
rtest17:
	$(DRIVER) -t ./traces/trace17.txt -s $(TSHREF) -a $(TSHARGS)
rtest18:
	$(DRIVER) -t ./traces/trace18.txt -s $(TSHREF) -a $(TSHARGS)

# clean up
clean:
	rm -f $(FILES) *.o *~
