#include "gwutils.h"
#include <iostream>


void gwutils::parse_sockaddr(struct sockaddr addr, socklen_t socklen){
   struct sockaddr_in *paddr = (sockaddr_in*)&addr;
   if (paddr->sin_family == AF_INET) {
    std::cout << "AF_INET "; 
   }
   char ip[128];
   inet_ntop(paddr->sin_family, &paddr->sin_addr, ip, sizeof(ip));
   std::cout << "IP : " << ip << " Port : " << ntohs(paddr->sin_port) << std::endl;

   return
}