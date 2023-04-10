#pragma once

#include <netinet/in.h>
#include <sys/_types/_in_port_t.h>
namespace chatroom {
namespace net {

class Socket {
public:
  Socket(const char *addr, in_port_t port);
  int start();
  int close();
  int accept();

private:
  sockaddr_in addr;
  int sock_fd;
};

} // namespace net
} // namespace chatroom