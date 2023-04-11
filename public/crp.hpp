#pragma once

#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <cstring>
#include <sys/_endian.h>
#include <sys/socket.h>
namespace chatroom {
namespace net {

enum OP_CODE {
  CHAT,
  CHAT_GROUP,
  FILE_START,
  FILE_MID,
  FILE_END,
  LOGIN,
  LOGOUT
};

// Chat Room Protocol报文
class CRPMessage {
public:
  //将message写入网络,传入buf和buf长度,成功返回写入长度，不成功返回-1
  int marshal(char *, int);
  //从网络还原message,传入buf和buf长度，成功返回读出长度，不成功返回-读出长度
  int unmarshal(char const *, int);
  uint16_t get_length();
  CRPMessage();
  CRPMessage(uint16_t length, uint8_t op_code, uint32_t sender,
             uint32_t receiver, char const *data);
  void DEBUG();
  static int peek_length(char const *buf) { return ntohs(*(uint16_t *)buf); }

private:
  // head
  uint16_t length;
  uint8_t op_code;
  uint32_t sender;
  uint32_t receiver;
  // uint8_t data[4096 - 11];

  // data
  uint8_t data[4096 - 11];
};

// CRP协议本体
class CRP {
public:
  CRP(int);
  CRPMessage *receive();
  int send(CRPMessage *);

private:
  int fd;
  char buf[4096];
  int pointer;
};

} // namespace net
} // namespace chatroom
