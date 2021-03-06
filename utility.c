#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


void signal_bind_wrapper(int signo, void (*func)(int)){
    struct sigaction * action = (struct sigaction*)malloc(sizeof(struct sigaction));
    action->sa_handler = func;
    sigemptyset(&(action->sa_mask));
    action->sa_flags = 0;
    sigaction(signo, action, NULL);
}


void start_timer(int sec, int usec){
    static struct itimerval tim;
    tim.it_value.tv_sec = sec;
    tim.it_value.tv_usec = usec;
    tim.it_interval.tv_sec = 0;
    tim.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &tim, NULL);
}


int bind_wrapper(int socket_no, char * ipaddress, int port_no, int reusable){
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr (ipaddress);
    serverAddr.sin_port = htons(port_no);
    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));
    int yes=1;
    if (reusable){
        if (setsockopt(socket_no,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes))==-1){
            perror("setsockopt");
            exit(1);
        }
    }
    return bind(socket_no,(struct sockaddr*) &serverAddr,sizeof(serverAddr));
}

int connect_wrapper(int socket_no, char *ipaddress, int port_no){
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port_no);
    serverAddr.sin_addr.s_addr = inet_addr(ipaddress);
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    size_t addr_size = sizeof serverAddr;
    if (connect(socket_no, (struct sockaddr *) &serverAddr, addr_size) == -1){
        perror("Failed to connect to the server");
        exit(2);
    }
    return 1;
}

size_t send_udp_wrapper(int socket_no, void * data, size_t size, char * ipaddress, int port_no){
    struct sockaddr_in server_udp;
    server_udp.sin_family = AF_INET;
    server_udp.sin_addr.s_addr = inet_addr(ipaddress);
    server_udp.sin_port = htons(port_no);
    return sendto(socket_no, data, size, 0, (struct sockaddr*)&server_udp, sizeof(struct sockaddr_in));
}