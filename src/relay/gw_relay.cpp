#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "gwtcp.h"
#include "gwcommon.h"


typedef struct relay_server_context {
    pthread_t relay_server_thread;
    int server_sockfd;
    int clientfd;
} relay_server_context_t;

typedef struct traffic_collection_context {
    pthread_t traffic_collection_thread;
} traffic_collection_context_t;
typedef struct relay_context {
    relay_server_context_t relay_server_context;
    traffic_collection_context_t traffic_collection_context;
}  relay_context_t;

relay_context_t g_relay_context;

static void kill_relay_server_context(){
    gwtcp::server_disconnect_client(g_relay_context.relay_server_context.clientfd);
    pthread_kill(g_relay_context.relay_server_context.relay_server_thread, SIGTERM);
}

void sighandler(int signal){
    kill_relay_server_context();
}

void* tcp_server(void *arg) {
    char *server_ip = (char*)arg;
    int cfd = gwtcp::server_get_clientfd(server_ip, GODWIT_PORT);
    g_relay_context.relay_server_context.clientfd = cfd;
    int i = 0;
    char buf[128];
    int bytessent = 0;
    if (cfd >=0) {
        while (true) {
            snprintf(buf, 128, "APOORV %d", i);
            bytessent = gwtcp::server_send_pkt_to_clientfd(cfd, buf, 128);
            if (bytessent <= 0) {
                perror ("End of data");
                gwtcp::server_disconnect_client(cfd);
                break;
            }
            std::cout << "SENT : " << buf << std::endl;
            i++;     
            sleep(1);
        }
    }
    return NULL;
}

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

int main(int argc, char *argv[]) {

    std::cout << "this is the relay" << std::endl;
    if (argc < 2) {
        std::cout << "provide server interface ip" << std::endl;
        exit(-1);
    }
    pthread_create(&g_relay_context.relay_server_context.relay_server_thread, NULL, &tcp_server, argv[1]);

    pthread_join(g_relay_context.relay_server_context.relay_server_thread, NULL);

    std::cout << "END" << std::endl;
    return 0;
}