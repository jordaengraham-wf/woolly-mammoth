/* CMPT 434 - Winter 2016
 * Assignment 1, A
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
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT "30490" /* the port client will be connecting to */
#define PROXYPORT "30491" /* the port client will be connecting to */
#define MAXDATASIZE 1001 /* the port client will be connecting to */


struct tuples {
    char *key;
    char *value;
};

struct nodes{
    struct tuples* tuple;
    struct nodes* next;
};
//char *usage = "\nUsage:\n\tadd key value : add (key, value) pair, if no existing pair with same key value\n"
//        "\tgetvalue key : return value from matching (key, value) pair, if any\n"
//        "\tgetall : return all (key, value) pairs\n"
//        "\tremove key : remove matching (key, value) pair, if any\n"
//        "\tquit : exit the server";

int start(char *port);
int get_connections(int server_fd);
char *recvMessage(int client_fd);
int sendMessage(int client_fd, char *message);
int connect_to_server(char *host, char *port);

#endif //CMPT434_SERVER_H
