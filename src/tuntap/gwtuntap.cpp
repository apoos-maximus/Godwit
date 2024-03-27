#include "gwtuntap.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define GW_ROUTE_FILE "gwroute.txt"

static int tun_alloc(char *dev) {
    struct ifreq ifr;
    int fd, err;
    
    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
    {
        std::cout << "failed to open tun device with fd -  " << fd << std::endl;
        exit(-1);
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN;
    if(*dev){
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) == -1) {
        perror("ioctl TUNSETIFF");
        close(fd);
        exit(-1);
    }

    std::cout << "tun device " << dev <<" created and connected" << std::endl;

    return fd;
}

int tuntap_create_tun_device(char *tun_dev_name) {
    char cmdbuf[512];
    snprintf(cmdbuf, sizeof(cmdbuf), "ip tuntap add dev %s mod tun", tun_dev_name);
    int ret = system(cmdbuf);

    if (ret != 0) {
        std::cout << "creating tun device " << tun_dev_name << " failed";
        exit(-1);
    }

    snprintf(cmdbuf, sizeof(cmdbuf), "ip link set dev %s up", tun_dev_name);
    ret = system(cmdbuf);

    if (ret != 0) {
        std::cout << "failed to link up tun device " << tun_dev_name << std::endl;
        exit(-1);
    }

    std::cout << "created tun device " << tun_dev_name << std::endl;
    return ret;
}


int tuntap_set_tun_dev_ip(char *tun_dev_name, char *tun_ip) {
    char cmdbuf[512];
    snprintf(cmdbuf, sizeof(cmdbuf), "ip addr add %s dev %s", tun_ip, tun_dev_name);
    int ret = system(cmdbuf);
    
    if (ret != 0) {
        std::cout << "failed to set ip on tun device " << tun_dev_name << std::endl;
        exit(-1);
    }

    std::cout << "set ip " << tun_ip << " on " << tun_dev_name << std::endl;
    return ret;
}

int tuntap_delete_tun_device(char *tun_dev_name) {
    char cmdbuf[512];
    snprintf(cmdbuf, sizeof(cmdbuf), "ip link delete %s", tun_dev_name);
    int ret = system(cmdbuf);
    
    if (ret != 0) {
        std::cout << "failed to delete tun device " << tun_dev_name << std::endl;
    }
    return ret;
}

static int tuntap_create_route(char *network_cidr, char *gateway, char *intf){

}

int tuntap_create_tun_route(char *tun_dev_name){
    FILE *rtfp = fopen(GW_ROUTE_FILE, "r+");
    if (rtfp == NULL) {
        std::cout << "failed to open " << GW_ROUTE_FILE << std::endl;
        exit(-1);
    }
    char rtline[512];
    fscanf(rtfp, "%s", rtline);
    while(feof(rtfp)){

    }

}