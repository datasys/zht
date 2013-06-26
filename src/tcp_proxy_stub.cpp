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

bool TCPProxy::send(const void *sendbuf, const size_t sendcount) {

	return false;
}

bool TCPProxy::recv(void *recvbuf, size_t &recvcount) {

	return false;
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
	recvcount = ::recv(sock, recvbuf, sizeof(recvbuf), 0); //todo: sizeof(recvbuf)

	if (recvcount < 0) {

		cerr << "net_util: generalReceive(): error on receive: "
				<< strerror(errno) << endl;
		return -1;
	}

	int recv_bool = recvcount >= 0;

	return sent_bool && recv_bool;
}

bool TCPProxy::teardown() {

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

/*HostEntity he;
 if (phe == NULL) {

 getHostByKey(msg, he);
 if (!he.host.empty())
 phe = &he;
 }

 if (phe == NULL)
 return Const::ZSC_REC_CLTFAIL; //-2

 int sock = getSockCached(phe, TCP);
 reuseSock(sock);

 ...send...
 int ssize = generalSendTo(phe->host.data(), phe->port, sock, msg.c_str(),
 msg.size(), TCP);
 */

/*

 const string ZHTClient::getDestZHTByKey(const HostEntity& coordinator,
 const string& msg, string& destZHT) {

 Package pkg;
 pkg.ParseFromString(msg);
 pkg.set_opcode(Const::ZSC_OPC_GET_DESTZHT);

 string result;
 string sstatus = sendPkgInternal(coordinator.host, coordinator.port,
 pkg.SerializeAsString(), result);

 Package pkg2;
 pkg2.ParseFromString(result);

 destZHT = pkg2.targetzht();

 return sstatus;
 }
 */

/*


 const string ZHTClient::getHostByKey(const string& msg, HostEntity& he) {

 Package pkg;
 pkg.ParseFromString(msg);

 int index = HashUtil::genHash(pkg.virtualpath()) % _MemList.size(); //todo: problem when shrink the size of ZHT network.

 string destZHT;
 string sstatus = getDestZHTByKey(_MemList.at(index), msg, destZHT);

 if (!destZHT.empty())
 he = getHostEntity(Address::getHost(destZHT),
 Address::getPort(destZHT));

 return sstatus;
 }



 int ZHTClient::getSockCached(const HostEntity* phe, const bool& tcp) {

 int sock = 0;

 if (phe == NULL)
 return sock;

 return getSockCached(phe->host, phe->port, tcp);

 }

 int ZHTClient::getSockCached(const string& host, const uint& port,
 const bool& tcp) {

 int sock = 0;

 string hashKey = HashUtil::genBase(host, port);

 if (tcp == true) {

 sock = CONN_CACHE.fetch(hashKey, tcp);

 if (sock <= 0) {

 sock = makeClientSocket(host.c_str(), port, tcp);

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
 } else {

 if (UDP_SOCKET <= 0) {
 sock = makeClientSocket(host.c_str(), port, tcp);
 UDP_SOCKET = sock;
 } else
 sock = UDP_SOCKET;
 }

 return sock;
 }



 int ZHTClient::tearDown() {

 int rcode = 0;

 try {
 if (TCP == true) {

 int size = _MemList.size();

 for (int i = 0; i < size; i++) {

 struct HostEntity dest = _MemList.at(i);
 int sock = dest.sock;

 if (sock > 0) {
 close(sock);
 }
 }
 }
 } catch (exception& e) {

 fprintf(stderr, "%s, exception caught:\n\t%s", "ZHTClient::tearDown",
 e.what());
 rcode = -1;
 }

 return rcode;
 }
 */

TCPStub::TCPStub() {

}

TCPStub::~TCPStub() {
}

bool TCPStub::send(const void *sendbuf, const size_t sendcount) {

	return false;
}

bool TCPStub::recv(void *recvbuf, size_t &recvcount) {

	return false;
}

bool TCPStub::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	return false;
}

bool TCPStub::teardown() {

	return true;
}
