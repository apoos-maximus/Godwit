#include<iostream>
#include <signal.h>
#include "gwtcp.h"
#include "gwcommon.h"

int sfd = 0;

void sighandler(int signal){
	gwtcp::client_disconnect_from_server(sfd);
}

/*
Godwit Client will have two threads :-
	1. Ingress Thread
		- Receiving all the traffic from the relay and enqueing it
			onto the lock-free queue.
	2. Egress Thread
		- Sending packets emitte by the local applications back to 
			the relay.
*/

int main(int argc , char *argv[]) {

	signal(SIGINT, sighandler);

	std::cout << "this is the client" << std::endl;

	

	sfd = gwtcp::client_connect_to_server(argv[1], GODWIT_PORT);

	char data[128];
	int recvbytes = 0;
	while (true){
		recvbytes = gwtcp::client_read_pkt_from_server(sfd, data, 128);
		if (recvbytes <= 0) {
			perror ("End of data");
			gwtcp::client_disconnect_from_server(sfd);
			break;
		}
		std::cout << "RECVD : " << data << std::endl;
	}
		
	

	return 0;
}