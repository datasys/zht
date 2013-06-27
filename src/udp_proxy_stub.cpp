/*
 * udp_proxy_stub.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "udp_proxy_stub.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>

#include "ZHTUtil.h"

int UDPProxy::UDP_SOCKET = -1;

UDPProxy::UDPProxy() {

}

UDPProxy::~UDPProxy() {
}

bool UDPProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	ZHTUtil zu;
	string msg((char*) sendbuf, sendcount);

	HostEntity he = zu.getHostEntityByKey(msg);

	int sock = getSockCached(he.host, he.port);

	reuseSock(sock);

	/*send over sock*/
	int sentSize = udpSendTo(sock, he.host, he.port, (char*) sendbuf,
			sendcount);

	int sent_bool = sentSize == sendcount;

	/*receive response*/
	recvcount = udpRecvFrom(sock, recvbuf, recvcount);

	int recv_bool = recvcount >= 0;

	return sent_bool && recv_bool;
}

int UDPProxy::udpSendTo(int sock, const string &host, uint port,
		const char* sendbuf, int sendcount) {

	sockaddr_in recvAddr;

	struct hostent *hp;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	hp = gethostbyname(host.c_str());

	bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
	server.sin_port = htons(port);

	int sentSize = sendto(sock, sendbuf, sendcount, 0,
			(struct sockaddr*) &server, sizeof(struct sockaddr));

	if (sentSize < 0) {
		cerr << "net_util.cpp: udpSendTo error: " << strerror(errno) << endl;
	}

	return sentSize;
}

int UDPProxy::udpRecvFrom(int sock, void* recvbuf, int recvbufsize) {

	struct sockaddr_in recvAddr;
	socklen_t addr_len = sizeof(struct sockaddr);

	int recvcount = recvfrom(sock, recvbuf, recvbufsize, 0,
			(struct sockaddr *) &recvAddr, &addr_len);

	if (recvcount < 0) {
		cerr << "net_util.cpp: udpRecvFrom error: " << strerror(errno) << endl;
	}

	return recvcount;
}

bool UDPProxy::teardown() {

	int rc = close(UDP_SOCKET);

	return rc == 0;
}

int UDPProxy::getSockCached(const string& host, const uint& port) {

	int sock;

	if (UDP_SOCKET <= 0) {

		sock = makeClientSocket(host, port);
		UDP_SOCKET = sock;

	} else {

		sock = UDP_SOCKET;
	}

	return sock;
}

int UDPProxy::makeClientSocket(const string& host, const uint& port) {

	int to_sock = 0;

	to_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (to_sock < 0) {

		printf("Error occurred when creating the socket:%d\n", to_sock);
		printf("%s\n", strerror(errno));
		close(to_sock);

		return -1;
	}

	return to_sock;
}

UDPStub::UDPStub() {

}

UDPStub::~UDPStub() {
}

bool UDPStub::recvsend(void *recvbuf, size_t &recvcount, const void *sendbuf,
		const size_t sendcount) {

	return true;
}
