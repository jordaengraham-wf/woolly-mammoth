/* CMPT 434 - Winter 2016
 * Assignment 1, Question 3
 *
 * Jordaen Graham - jhg257
 *
 * File: server.h
 */


#ifndef CMPT434_SERVER_H
#define CMPT434_SERVER_H

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct tuples {
    char *key;
    char *value;
};

struct nodes{
    struct tuples* tuple;
    struct nodes* next;
};


#define PORT "4951"    // the port users will be connecting to
#define PROXYPORT "4952"    // the port users will be connecting to
#define MAXDATASIZE 1000

int start(char *port);
int get_connections(int server_fd);
char *recvTCPMessage(int client_fd);
int sendTCPMessage(int client_fd, char *message);

#endif //CMPT434_SERVER_H
