# Godwit

A Small backyard project about packet-forwarding.

## Design 
### 1. Mechanism to collect traffic at the Public Godwit_Relay
a. Tun Interface

The traffic destined for the private network arrives at the public internet facing ethernet interface and is redirected to the tun interface with the help of PREROUTING chain in the "nat" table.

The ip packets however greater than the MTU of the ethernet interface get fragmented and we don't want to deal with fragmentation in our user-space program. Fragmentation will occur but we want to design everything in such a way that the native operating system kernels take care of it for us.

```
    sudo ip tuntap add dev tun0 mode tun
    sudo ip addr add 10.10.12.1/24 dev tun0
    sudo ip link set up dev tun0
```
>Note : Run tunread program to make interface active
```
//tunread.c
include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define IFNAMSIZ 16

int main() {
  int tun_fd = open("/dev/net/tun", O_RDWR);
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
  strcpy(ifr.ifr_name, "tun0");
  ioctl(tun_fd, TUNSETIFF, (void *)&ifr);

  while (1) {
    char buffer[1500];
    int nread = read(tun_fd, buffer, sizeof(buffer));
    printf("Read %d bytes from device %s\n", nread, ifr.ifr_name);
  }

  close(tun_fd);
  return 0;
}
```

```
root@juice-box:/home/apoorv# iptables \
> --table nat \
> --append PREROUTING \
> --protocol udp \
> --jump DNAT \
> --to-destination 10.10.12.2
```
> Note : the destination ip is not the tun ip else the packet will get absorbed.



