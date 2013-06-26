/*
 * net_util.h
 *
 *  Created on: Feb 15, 2012
 *      Author: tony
 */

#ifndef NET_UTIL_H_
#define NET_UTIL_H_

#include "stddef.h"
#include <sys/types.h>

#include <vector>
#include <map>
#include <set>
#include <queue>
#include "meta.pb.h"
using namespace std;

int makeSvrSocket(int port, bool tcp);
int svr_accept(int sock, bool tcp);
int makeClientSocket(const char* host, int port, bool tcp);

int generalSendTo(const char* host, int port, int to_sock, const char* buf,
		int bufsize, bool tcp);

ssize_t sendDataBySize(const int& to_sock, const char *byteBuf,
		const ssize_t& bytesToSend);

ssize_t sendWholeData(const int& to_sock, const char *byteBuf,
		const size_t& bytesToSend);

int generalSendBack(int to_sock, const char* buf, int bufsize,
		struct sockaddr_in sendbackAddr, int flag, bool tcp);

int generalReceive(int sock, void* buf, int bufsize,
		struct sockaddr_in & recvAddr, int flag, bool tcp);

int loopedReceive(int sock, string& srecv, struct sockaddr_in & recvAddr,
		int flag, bool tcp);

int generalSendTCP(int to_sock, const char* buf, int bufsize);
int generalReveiveTCP(int sock, void *buf, size_t bufsize, int flags);

ssize_t recvDataSize(const int& from_sock, size_t& bytesToRead);

ssize_t recvDataBySize(const int& from_sock, char *byteBuf, size_t bytesToRead);

ssize_t recvWholeData(const int& from_sock, char *byteBuf,
		const size_t& bytesToRead);

//int serverReceive(int sock, void *buffer, size_t size, int flags, bool tcp);

int udpSendTo(int toSock, const char* host, int port, const char* buf,
		int bufsize);
int udpRecvFrom(int sock, void* buf, int bufsize, struct sockaddr_in & recvAddr,
		int flag);
int udpSendBack(int sock, const char* buf, int bufsize,
		struct sockaddr_in sendbackAddr, int flag);
int reuseSock(int sock);
int setRecvTimeout(int sockfd, unsigned int sec, unsigned int usec);
