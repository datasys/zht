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
#include "HTWorker.h"

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

	/*get client sock fd*/
	ZHTUtil zu;
	string msg((char*) sendbuf, sendcount);
	HostEntity he = zu.getHostEntityByKey(msg);

	int sock = getSockCached(he.host, he.port);

	reuseSock(sock);

	/*send message to server over client sock fd*/
	int sentSize = sendTo(sock, sendbuf, sendcount);
	int sent_bool = sentSize == sendcount;

	/*receive response from server over client sock fd*/
	recvcount = recvFrom(sock, recvbuf, recvcount);
	int recv_bool = recvcount >= 0;

	/*combine flags as value to be returned*/
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

			cerr
					<< "TCPProxy::getSockCached(): error on makeClientSocket(...): "
					<< strerror(errno) << endl;
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

		cerr << "TCPProxy::makeClientSocket(): error on ::socket(...): "
				<< strerror(errno) << endl;
		return -1;
	}

	int ret_con = connect(to_sock, (struct sockaddr *) &dest, sizeof(sockaddr));

	if (ret_con < 0) {

		cerr << "TCPProxy::makeClientSocket(): error on ::connect(...): "
				<< strerror(errno) << endl;
		return -1;
	}

	return to_sock;

}

int TCPProxy::sendTo(int sock, const void* sendbuf, int sendcount) {

	BdSendBase *pbsb = new BdSendToServer((char*) sendbuf);
	int sentSize = pbsb->bsend(sock);
	delete pbsb;
	pbsb = NULL;

	/*prompt errors*/
	if (sentSize < sendcount) {

		cerr << "TCPProxy::sendTo(): error on BdSendToServer::bsend(...): "
				<< strerror(errno) << endl;
	}

	return sentSize;
}

int TCPProxy::recvFrom(int sock, void* recvbuf, int recvbufsize) {

	//todo: loopedReceive for zht_lookup
	int recvcount = ::recv(sock, recvbuf, recvbufsize, 0);

	/*prompt errors*/
	if (recvcount < 0) {

		cerr << "TCPProxy::recvFrom: error on ::recv(...): " << strerror(errno)
				<< endl;
	}

	return recvcount;
}

TCPStub::TCPStub() {

}

TCPStub::~TCPStub() {
}

bool TCPStub::recvsend(ProtoAddr addr, const void *recvbuf) {

	/*get response to be sent to client*/
	HTWorker htw;
	string result = htw.run((char*) recvbuf);

	const char *sendbuf = result.data();
	int sendcount = result.size();

	/*send response to client over server sock fd*/
	int sentsize = sendBack(addr, sendbuf, result.size());
	bool sent_bool = sentsize == sendcount;

	return sent_bool;
}

int TCPStub::sendBack(ProtoAddr addr, const void* sendbuf, int sendcount) {

	/*send response to client over server sock fd*/
	BdSendBase *pbsb = new BdSendToClient((char*) sendbuf);
	int sentsize = pbsb->bsend(addr.fd);

	delete pbsb;
	pbsb = NULL;

	/*prompt errors*/
	if (sentsize < sendcount) {

		cerr << "TCPStub::sendBack():  error on BdSendToClient::bsend(...): "
				<< strerror(errno) << endl;
	}

	return sentsize;
}
