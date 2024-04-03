#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace gwutils {
    void parse_sockaddr(struct sockaddr *addr, socklen_t socklen);
}