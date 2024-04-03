#include <iostream>
#include "gwtcp.h"
/*
 Relay will have three threads for now :- 
 1. Ingress Producer Thread : 
   - Consuming all the tcp/udp traffic from the relay and enquing it 
       into the ingress-lockfree queue.
 2. Ingress Consumer / Egress Producer Thread :
   A. Ingress
       - Dequeing the IP packets from the ingress lock-free queue.
       - Doing necessary packet processing, like DNAT-ing
       - Sending the processed packet over to the client
           in the private network.
   B. Egress
       - Receiving packets from the client.
       - Doing necessary packet processing like SNAT-ing
           and enqueing into the egress-queue.

 3. Egress Consumer Thread
    - Dequing the IP packets from the egress queue and sending them 
        out to the internet to complete tcp/udp transaction.
*/

int main() {

    std::cout << "this is the relay" << std::endl;

    int cfd = gwtcp::server_get_clientfd("10.0.0.2", 8008);

    if (cfd >=0) {
        gwtcp::send_pkt_to_clientfd(cfd, "APOORV", 10);
    }

    return 0;
}