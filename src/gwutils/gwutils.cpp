#include "gwutils.h"
#include <iostream>


void gwutils::parse_sockaddr(struct sockaddr *addr, socklen_t socklen){
   struct sockaddr_in *paddr = (sockaddr_in *)addr;
   // uint32_t naddr = paddr->sin_addr.s_addr;
   // std::cout << "AF : " << paddr->sin_family << " IP : " << naddr << " Port : " << ntohs(paddr->sin_port) << std::endl;
   
   if (paddr->sin_family == AF_INET) {
    std::cout << "AF_INET "; 
   }
   char ip[128] = {'\0'};
   inet_ntop(paddr->sin_family, &paddr->sin_addr.s_addr, ip, 128);
   std::cout << "IP : " << ip << " Port : " << ntohs(paddr->sin_port) << std::endl;

   return;
}