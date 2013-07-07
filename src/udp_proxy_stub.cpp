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
#include "HTWorker.h"

int UDPProxy::UDP_SOCKET = -1;

UDPProxy::UDPProxy() {

}

UDPProxy::~UDPProxy() {
}

bool UDPProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	/*get client sock fd*/
	ZHTUtil zu;
	string msg((char*) sendbuf, sendcount);

	HostEntity he = zu.getHostEntityByKey(msg);

	int sock = getSockCached(he.host, he.port);

	reuseSock(sock);

	/*send message to server over client sock fd*/
	int sentSize = sendTo(sock, he.host, he.port, (char*) sendbuf, sendcount);
	int sent_bool = sentSize == sendcount;

	/*receive response from server over client sock fd*/ //todo: loopedReceive for zht_lookup
	recvcount = recvFrom(sock, recvbuf, recvcount);
	int recv_bool = recvcount >= 0;

	/*combine flags as value to be returned*/
	return sent_bool && recv_bool;
}

int UDPProxy::sendTo(int sock, const string &host, uint port,
		const void* sendbuf, int sendcount) {

	sockaddr_in recvAddr;

	struct hostent *hp;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	hp = gethostbyname(host.c_str());

	bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
	server.sin_port = htons(port);

	int sentSize = sendto(sock, sendbuf, sendcount, 0,
			(struct sockaddr*) &server, sizeof(struct sockaddr));

	/*prompt errors*/
	if (sentSize < sendcount) {

		cerr << "UDPProxy::sendTo(): error on ::sendto(...): "
				<< strerror(errno) << endl;
	}

	return sentSize;
}

int UDPProxy::recvFrom(int sock, void* recvbuf, int recvbufsize) {

	struct sockaddr_in recvAddr;
	socklen_t addr_len = sizeof(struct sockaddr);

	int recvcount = recvfrom(sock, recvbuf, recvbufsize, 0,
			(struct sockaddr *) &recvAddr, &addr_len);

	/*prompt errors*/
	if (recvcount < 0) {

		cerr << "UDPProxy::recvFrom(): error on ::recvfrom(...): "
				<< strerror(errno) << endl;
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

bool UDPStub::recvsend(ProtoAddr addr, const void *recvbuf) {

	/*get response to be sent to client*/
	HTWorker htw;
	string result = htw.run((char*) recvbuf);

	const char *sendbuf = result.data();
	int sendcount = result.size();

	/*send response to client over server sock fd*/
	int sentsize = sendBack(addr, sendbuf, sendcount);
	bool sent_bool = sentsize == sendcount;

	return sent_bool;
}

int UDPStub::sendBack(ProtoAddr addr, const void* sendbuf, int sendcount) {

	int sentsize = sendto(addr.fd, sendbuf, sendcount, 0,
			(struct sockaddr *) addr.sender, sizeof(struct sockaddr));

	if (sentsize < sendcount) {

		cerr << "UDPStub::sendBack(): error on ::sendto(...)" << strerror(errno)
				<< endl;
	}
	return sentsize;
}
