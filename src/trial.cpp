#include "tuntap/gwtuntap.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define MAX_IP_PKTSIZE 65535
#define GODWIT_TUN "godwtun0"
#define GODWIT_TUN_IP "10.10.20.1/24"
pthread_t persist_thread;

void* thread_func(void * arg){
    for(;;){

    }
}

void sighandler(int signal){
    tuntap_delete_tun_device(GODWIT_TUN);
    pthread_kill(persist_thread, SIGTERM);
}

int main(int argc, char *argv[]){

    // if (argc < 2) {
    //     std::cout << "interface name required" << std::endl;
    //     exit(-1);
    // }


    // std::cout << "Trial File" << std::endl;

    // int status = 0;
    // int ipsockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    // if (ipsockfd < 0) {
    //     std::cout << "socket() - faield with " << ipsockfd << std::endl;
    //     exit(-1);
    // } else {
    //     std::cout << "socket() - OK - with " << ipsockfd << std::endl;
    // }

    // struct ifreq ifr;
    // int  len  = strnlen(argv[1], IFNAMSIZ);
    // if (len == IFNAMSIZ){
    //     std::cout << "too long interface name" << std::endl;
    //     exit(-1);
    // }

    // memset(&ifr, 0, sizeof(ifr));
    // snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), argv[1]);

    // if ((status = setsockopt(ipsockfd, SOL_SOCKET, SO_BINDTODEVICE, (void*)&ifr, sizeof(ifr))) < 0) {
    //     std::cout << "setsockopt() - Failed with " << status << std::endl;
    // } else {
    //     std::cout << "setsockopt() - OK with " << status << std::endl;
    // }



    // char *recvbuf = (char*)calloc(MAX_IP_PKTSIZE, sizeof(char));
    // if (recvbuf == NULL) {
    //     std::cout << "memory allocation for buffer failed" << std::endl;
    // } else {
    //     std::cout << "allcated receive buffer " << recvbuf << std::endl;
    // }

    // struct sockaddr peeraddr;
    // memset(&peeraddr, 0, sizeof(peeraddr));
    // socklen_t peeraddrlen = 0;
    // int nrecvbytes = 0;
    
    // while (true){
    //     nrecvbytes = recv(ipsockfd, recvbuf, MAX_IP_PKTSIZE, 0);
    //     std::cout << "Received " << nrecvbytes << std::endl;
    // }

    

    pthread_create(&persist_thread, NULL, &thread_func, NULL);
    signal(SIGINT, sighandler);

    tuntap_create_tun_device(GODWIT_TUN);
    tuntap_set_tun_dev_ip(GODWIT_TUN, GODWIT_TUN_IP);

    

    pthread_join(persist_thread, NULL);
    return 0;
}