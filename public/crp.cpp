
#include "crp.hpp"
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/_endian.h>
#include <unistd.h>

namespace chatroom {
namespace net {

int CRPMessage::unmarshal(char const *buf, int buflen) {
  length = ntohs(*((uint16_t *)buf));
  std::cout << "unmarshal length: " << length << std::endl;
  if (buflen < length) {
    return -length;
  } else {
    op_code = buf[2];
    sender = ntohl(*(uint32_t *)(buf + 3));
    receiver = ntohl(*(uint32_t *)(buf + 7));
    memcpy(data, buf + 11, length - 11);
  }

  return length;
}

int CRPMessage::marshal(char *buf, int buflen) {
  if (length > buflen) {
    return -1;
  }

  *((uint16_t *)buf) = htons(length);
  *((uint8_t *)(buf + 2)) = op_code;
  *((uint32_t *)(buf + 3)) = htonl(sender);
  *((uint32_t *)(buf + 7)) = htonl(receiver);
  memcpy(buf + 11, data, length - 11);

  return length;
}

uint16_t CRPMessage::get_length() { return length; }

CRPMessage::CRPMessage() { memset(this, 0, sizeof(CRPMessage)); }

CRPMessage::CRPMessage(uint16_t length, uint8_t op_code, uint32_t sender,
                       uint32_t receiver, char const *data)
    : length(length), op_code(op_code), sender(sender), receiver(receiver) {
  memcpy(this->data, data, length - 11);
}

void CRPMessage::DEBUG() {
  // std::cout << "{\n"
  //           << "\tlength: " << length << "\n"
  //           << "\top_code: " << (int)op_code << "\n"
  //           << "\tsender: " << sender << "\n"
  //           << "\treceiver: " << receiver << "\n"
  //           << "\tdata: " << data << "\n"
  //           << "}\n";
  printf("{\n\tlength: %d\n\top_code: %d\n\tsender: %d\n\treceiver: "
         "%d\n\tdata: %s\n}\n",
         length, (int)op_code, sender, receiver, data);
}

CRP::CRP(int fd) : fd(fd), pointer(0) {}

int CRP::receive(CRPMessage *message) {
  int n = recv(fd, buf + pointer, 4096 - pointer, 0);
  if (n == 0 && pointer == 0) {
    return -1;
  }

  if (n > 0) {
    pointer += n;
  }

  std::cout << "recv: " << n << std::endl;
  if (pointer < 2) {
    return 1;
  }
  int len = CRPMessage::peek_length(buf);
  if (pointer < len) {
    return 1;
  }

  message->unmarshal(buf, 4096);

  if (pointer > len) {
    memcpy(buf, buf + len, pointer);
  }
  pointer -= len;

  return 0;
}

int CRP::send(CRPMessage *msg) {
  msg->marshal(buf, 4096);
  return ::send(fd, buf, msg->get_length(), 0);
}

int CRP::close() { return ::close(fd); }

int CRP::get_fd() { return fd; }
} // namespace net
} // namespace chatroom