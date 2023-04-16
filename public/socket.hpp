#pragma once

#include <netinet/in.h>
#include <sys/_types/_in_port_t.h>
namespace chatroom {
namespace net {

class SocketStream {
public:
  //关闭socket
  int close();
  //构造函数
  SocketStream(const char *addr, in_port_t port);
  int get_sock_fd();

protected:
  sockaddr_in addr;
  int sock_fd;
};

class SocketStreamClient : public SocketStream {
public:
  int connect();
  SocketStreamClient(const char *addr, in_port_t port);
};
class SocketStreamHost : public SocketStream {
public:
  //开启socket
  int host();
  //接受连接返回fd
  int accept();
  SocketStreamHost(const char *addr, in_port_t port);
};

} // namespace net
} // namespace chatroom