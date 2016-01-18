# CMPT 434 - Winter 2016
# Assignment 1, Question 3
#
# Jordaen Graham - jhg257
#
# File: Makefile

CC := gcc
CCFLAGS := -Wall -Wextra -pedantic -pthread -g

all: clean UDP Client

clean:
	@rm *.o* &> /dev/null || true
	@rm *~ &> /dev/null || true
	@rm Client &> /dev/null || true
	@rm UDP &> /dev/null || true

RUN: UDP Client
	./UDP &
	./Client localhost test

run_udp: UDP
	./UDP

run_client: Client
	./Client localhost test

UDP: udp.c
	$(CC) $(CCFLAGS) -o UDP udp.c

Client: client.c
	$(CC) $(CCFLAGS) -o Client client.c


