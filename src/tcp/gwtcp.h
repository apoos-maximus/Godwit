
namespace gwtcp {

    int server_get_clientfd(char *intf_ip, int port);
    int send_pkt_to_clientfd(int client_fd, char *pktbuf,  int pktlen);

    int client_connect(char *serv_ip, int serv_port);
    int read_pkt_from_server(int servfd, char *pktbuf, int buflen);

}