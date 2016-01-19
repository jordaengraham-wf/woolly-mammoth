
#include "server.h"

char *recvTCPMessage(int client_fd){
    ssize_t numbytes;
    char *buf = malloc(MAXDATASIZE * sizeof(char));

    if ((numbytes = recv(client_fd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    else if (numbytes == 0){
        buf = "abort";
    }
    else
        buf[numbytes] = '\0';
    return buf;
}

int sendTCPMessage(int client_fd, char *message) {
    if (send(client_fd, message, strlen(message)+1, 0) == -1) {
        fprintf(stdout, "Error: sending to %d\n", client_fd);
        return -1;
    }
    return 0;
}

/* get sockaddr, IPv4 or IPv6: */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void sigchld_handler(int s) {
    /* waitpid() might overwrite errno, so we save and restore it: */
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

int get_connections(int server_fd) {
    int client_fd; /*return */
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];
    struct sockaddr_storage their_addr; /* connector's address information */

    sin_size = sizeof their_addr;
    client_fd = accept(server_fd, (struct sockaddr *)&their_addr, &sin_size);
    if (client_fd == -1) {
        perror("accept");
        return client_fd;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), (char *) s, INET6_ADDRSTRLEN);

    printf("server: got connection from %s\n", s);
    fflush(stdout);

    return client_fd;
}

int *setup(const char *port){
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;

    /* listen for connection on server_fd, communicates on client_fd */
    int rv, yes = 1, *server_fd = NULL;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; /* use my IP */

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    server_fd = malloc(sizeof(int));
    /* loop through all the results and bind to the first we can */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((*server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(*server_fd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*server_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); /* all done with this structure */

    if (p == NULL) {
        perror("server: failed to bind");
        exit(1);
    }

    if (listen(*server_fd, 10) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; /* reap all dead processes */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    return server_fd;
}

int start(char *port){
    struct ifaddrs *addrs, *tmp;
    struct sockaddr_in *pAddr = NULL;
    int *client_fd = NULL;

    client_fd = setup(port);
    getifaddrs(&addrs);
    tmp = addrs;
    printf("Addresses: [\n");
    while (tmp)
    {
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
        {
            pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            printf("\t%s, %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
        }
        tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
    printf("]\n");

    signal(SIGPIPE, SIG_IGN);
    printf("server: waiting for Clients on Port: %s\n", port);
    fflush(stdout);
    return *client_fd;
}
