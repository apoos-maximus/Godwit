
namespace gwtcp {

    int server_get_clientfd(char *intf_ip, int port);
    int server_send_pkt_to_clientfd(int client_fd, char *pktbuf,  int pktlen);
    int server_disconnect_client(int clientfd);

    int client_connect_to_server(char *serv_ip, int serv_port);
    int client_read_pkt_from_server(int servfd, char *pktbuf, int buflen);
    int client_disconnect_from_server(int servfd);
}