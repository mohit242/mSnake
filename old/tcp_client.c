#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 2000

int main(){
    int clientSocket;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    char name[10];
    scanf("%s", name);
    // printf("waiting for the connection\n");

    /*  Create the network socket.   */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*  1) PR_INET Internet domain
    2) SOCK_STREAM represents Stream socket
    3) 0 represents Default protocol (TCP in this case)
    */

    // Configure settings of the server address
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;

    /* Set port number */
    serverAddr.sin_port = htons(7891);
    /*  In networking we prefer to use big-endian binary 
    sequencing.  Since in our PC we use small endian binary
    scheme, we use htons to convert small endian to big
    endian.  */

    /* Set IP address to localhost */
    char ip_addr[] = "172.17.46.179";
    serverAddr.sin_addr.s_addr = inet_addr(ip_addr);

    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof
    serverAddr.sin_zero);

    /* Connect the socket to the server using the address*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
    int ret = sendto(clientSocket, name, sizeof(name), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    /* Read the message from the server into the buffer */
    // recv(clientSocket, buffer, 1024, 0);
    // int num = atoi(buffer);
    // printf("%d\n", num);
    sleep(10);
    for(int i = 0;i < 2; i++){
        ret = recvfrom(clientSocket, buffer, BUF_SIZE, 0, NULL, NULL);
        fputs(buffer, stdout);
        printf("\n");
    }
}
