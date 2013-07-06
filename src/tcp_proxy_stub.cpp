/*
 * tcp_proxy_stub.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "tcp_proxy_stub.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "Util.h"
#include "ZHTUtil.h"
#include "bigdata_transfer.h"

using namespace iit::datasys::zht::dm;

int TCPProxy::CACHE_SIZE = 1024;

LRUCache<string, int> TCPProxy::CONN_CACHE = LRUCache<string, int>(
		TCPProxy::CACHE_SIZE);

TCPProxy::TCPProxy() {

}

TCPProxy::~TCPProxy() {
}

bool TCPProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	ZHTUtil zu;
	string msg((char*) sendbuf, sendcount);
	HostEntity he = zu.getHostEntityByKey(msg);

	int sock = getSockCached(he.host, he.port);

	reuseSock(sock);

	/*send over sock*/
	BdSendBase *pbsb = new BdSendToServer((char*) sendbuf);
	int sentSize = pbsb->bsend(sock);
	delete pbsb;
	pbsb = NULL;

	int sent_bool = sentSize == sendcount;

	/*receive response*/ //todo: loopedReceive for zht_lookup
	recvcount = ::recv(sock, recvbuf, recvcount, 0);

	if (recvcount < 0) {

		cerr << "net_util: generalReceive(): error on receive: "
				<< strerror(errno) << endl;
		return -1;
	}

	int recv_bool = recvcount >= 0;

	return sent_bool && recv_bool;
}

bool TCPProxy::teardown() {

	//todo: close all socket cached
	return true;
}

int TCPProxy::getSockCached(const string& host, const uint& port) {

	int sock = 0;

	string hashKey = HashUtil::genBase(host, port);

	sock = CONN_CACHE.fetch(hashKey, true);

	if (sock <= 0) {

		sock = makeClientSocket(host, port);

		if (sock <= 0) {

			cerr << "Client insert:making connection failed." << endl;
			sock = -1;
		} else {

			int tobeRemoved = -1;
			CONN_CACHE.insert(hashKey, sock, tobeRemoved);

			if (tobeRemoved != -1) {
				close(tobeRemoved);
			}
		}
	}

	return sock;
}

int TCPProxy::makeClientSocket(const string& host, const uint& port) {

	int to_sock = 0;

	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(struct sockaddr_in)); /*zero the struct*/

	struct hostent * hinfo = gethostbyname(host.c_str());
	if (hinfo == NULL)
		printf("getbyname failed!\n");

	dest.sin_family = PF_INET; /*storing the server info in sockaddr_in structure*/
	dest.sin_addr = *(struct in_addr *) (hinfo->h_addr); /*set destination IP number*/
	dest.sin_port = htons(port);

	to_sock = socket(PF_INET, SOCK_STREAM, 0); //try change here.................................................

	if (to_sock < 0) {

		cerr << "net_util: error on socket(): " << strerror(errno) << endl;
		return -1;
	}

	int ret_con = connect(to_sock, (struct sockaddr *) &dest, sizeof(sockaddr));

	if (ret_con < 0) {

		cerr << "net_util: error on connect(): " << strerror(errno) << endl;
		return -1;
	}

	return to_sock;

}

TCPStub::TCPStub() {

}

TCPStub::~TCPStub() {
}

bool TCPStub::recvsend(ProtoAddr addr, const void * const recvbuf) {

	return true;
}
