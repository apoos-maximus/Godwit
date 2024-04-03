#include "gwtcp.h"
#include "../gwutils/gwutils.h"
#include <iostream>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define TCP_SERVER_QLEN 10
#define TCP_CLIENT_MAXSLEEP 128

static int 
initserver (int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;
    int err;
    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return (-1);
    if (bind(fd,  addr, alen) < 0)
        goto errout;
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
        if (listen(fd, qlen) < 0)
            goto errout;
    }
    return(fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return (-1);
}

int gwtcp::server_get_clientfd(char *intf_ip, int port){

    struct sockaddr servaddr;
    socklen_t servaddrlen = 0;

    struct sockaddr clientaddr;
    socklen_t clientaddrlen = 0;


    struct sockaddr_in *pservaddr = (sockaddr_in*)&servaddr; 
    pservaddr->sin_port  = htons(port);
    inet_pton(AF_INET, intf_ip, &pservaddr->sin_addr);
    pservaddr->sin_family = AF_INET;
    servaddrlen = sizeof(servaddr);

    int servfd = initserver(SOCK_STREAM, &servaddr, servaddrlen, TCP_SERVER_QLEN);
    if (servfd < 0) {
        perror("Error opening server socket");
        exit (-1);
    } else {
        int clfd = accept (servfd, &clientaddr, &clientaddrlen);
        if (clfd < 0) {
            std::cout << "Failed to accept client connection " << clfd << std::endl;
            exit (-1); 
        } else {
            std::cout << "Accepted client connection " << clfd << std::endl;
            gwutils::parse_sockaddr(clientaddr, clientaddrlen);
            return clfd;
        }
    }

}

int gwtcp::send_pkt_to_clientfd(int client_fd, char *pktbuf,  int pktlen) {
    return send(client_fd, pktbuf, pktlen, 0);
}

static 
int 
connect_retry(int domain, int type, int protocol,
              const struct sockaddr *addr, socklen_t alen)
{

    int numsec, fd;

    /*
     * Trying to connect with exponential backoff
     */
    
    for (numsec = 1; numsec <= TCP_CLIENT_MAXSLEEP; numsec <<= 1) {
        if ((fd = socket(domain, type, protocol)) < 0)
            return (-1);
        
        if (connect(fd, addr, alen) == 0) {
            /*
             * Connection accepted.
             */
            return (fd);
        }

        close(fd);

        /*
         * Delay before trying again
         */
        if (numsec <= TCP_CLIENT_MAXSLEEP/2)
            sleep(numsec);

    }

    return (-1);
}

int gwtcp::client_connect(char *serv_ip, int serv_port) {
    struct sockaddr servaddr;
    socklen_t servaddrlen = 0;

    struct sockaddr clientaddr;
    socklen_t clientaddrlen = 0;


    struct sockaddr_in *pservaddr = (sockaddr_in*)&servaddr; 
    pservaddr->sin_port  = htons(serv_port);
    inet_pton(AF_INET, serv_ip, &pservaddr->sin_addr);
    pservaddr->sin_family = AF_INET;
    servaddrlen = sizeof(servaddr);

    int sfd = 0;
    sfd = connect_retry(AF_INET, SOCK_STREAM, 0, &servaddr, servaddrlen);

    if (sfd < 0) {
        std::cout << "Error connecting to relay " << sfd << std::endl;
        exit (-1);
    } else {
        std::cout << "Connected to relay " << sfd << std::endl;
        return sfd;
    }
}

int gwtcp::read_pkt_from_server(int servfd, char *pktbuf, int buflen){
   return recv(servfd, pktbuf, buflen, 0);
}
