/*
 * net_util.cpp
 *
 *  Created on: Feb 15, 2012
 *      Author: tony
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include "net_util.h"
#include "zht_util.h"

#include "bigdata_transfer.h"

#define SOCKET_ERROR -1

class BdRecvBase;
class BdRecvFromServer;
template<class ID> class AckQueue;

AckQueue<uint64_t> ACK_QUEUE;

//make socket for server, include bind and listen(if TCP), return socket
int makeSvrSocket(int port, bool tcp) { //only for svr
	struct sockaddr_in svrAdd_in; /* socket info about our server */
	int svrSock = -1;

	try {
		memset(&svrAdd_in, 0, sizeof(struct sockaddr_in)); /* zero the struct before filling the fields */
		svrAdd_in.sin_family = AF_INET; /* set the type of connection to TCP/IP */
		svrAdd_in.sin_addr.s_addr = INADDR_ANY; /* set our address to any interface */
		svrAdd_in.sin_port = htons(port); /* set the server port number */

		if (tcp == true) { //make socket
			svrSock = socket(AF_INET, SOCK_STREAM, 0); /* OS will return a fd for network stream connection*/
		} else { //UDP

			svrSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		}

		if (svrSock < 0) {
			printf(
					"Error occurred when creating the socket:%d to the server port:%d\n",
					svrSock, port);
			printf("%s\n", strerror(errno));
			close(svrSock);
			return -1;
		}

		if (tcp == true) {
			//for TCP works. may not work for UDP
			if (bind(svrSock, (struct sockaddr *) &svrAdd_in,
					sizeof(struct sockaddr)) < 0) {
				printf(
						"Error occurred binding the socket:%d to the server port:%d\n",
						svrSock, port);
				printf("%s", strerror(errno));
				close(svrSock);
				return -1;
			}
		} else { //UDP bind: exactly same as TCP??
//		memset(&svrAdd_in, 0, sizeof(svrAdd_in));
//		svrAdd_in.sin_family = AF_INET;
//		svrAdd_in.sin_addr.s_addr = INADDR_ANY;
//		svrAdd_in.sin_port = htons(port);

			if (bind(svrSock, (struct sockaddr*) &svrAdd_in,
					sizeof(struct sockaddr)) < 0) {
				/*		printf(	"Error occurred when binding the socket:%d to the server port:%d\n",sock, port);
				 printf("%s\n", strerror(errno));*/
				close(svrSock);
				return -1;
			}
		}

		if (tcp == true) { //TCP needs listen, UDP does not.
			/* start listening, allowing a queue of up to 1 pending connection */
			if (listen(svrSock, 1000) < 0) {
				printf(
						"Error occurred while enabling listen on the socket:%d\n",
						svrSock);
				printf("%s", strerror(errno));
				close(svrSock);
				return -1;
			}
		}
	} catch (exception& e) {

		fprintf(stderr, "exception caught:\n\t%s", e.what());
	}

	return svrSock;
}

//accept request, return working socket
int svr_accept(int sock, bool tcp) {

	if (tcp == true) {
		struct sockaddr_in dest; /* socket info about the machine connecting to us */
		int socksize = sizeof(struct sockaddr_in);
		int consocket;
		consocket = accept(sock, (struct sockaddr *) &dest,
				(socklen_t *) &socksize);
		if (consocket < 0) {
			printf("Error occurred while accepting on the socket:%d\n", sock);
			printf("%s\n", strerror(errno));
		}
		return consocket;
	} else {
		return sock;
	}
}

//make socket for client to send/receive
int makeClientSocket(const char* host, int port, bool tcp) {
	int to_sock = 1;

	try {
		if (tcp == true) {
			struct sockaddr_in dest;
			memset(&dest, 0, sizeof(struct sockaddr_in)); /*zero the struct*/
			struct hostent * hinfo = gethostbyname(host);
			if (hinfo == NULL)
				printf("getbyname failed!\n");
			dest.sin_family = PF_INET; /*storing the server info in sockaddr_in structure*/
			dest.sin_addr = *(struct in_addr *) (hinfo->h_addr); /*set destination IP number*/
			dest.sin_port = htons(port);
			to_sock = socket(PF_INET, SOCK_STREAM, 0); //try change here.................................................
			if (to_sock < 0) {
				cerr << "net_util: error on socket(): " << strerror(errno)
						<< endl;
				return -1;
			}

			int ret_con = connect(to_sock, (struct sockaddr *) &dest,
					sizeof(sockaddr));
			if (ret_con < 0) {
				cerr << "net_util: error on connect(): " << strerror(errno)
						<< endl;
				return -1;
			}

		} else { //UDP
			to_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (to_sock < 0) {
				printf("Error occurred when creating the socket:%d\n", to_sock);
				printf("%s\n", strerror(errno));
				close(to_sock);
				return -1;
			}
		}
	} catch (exception& e) {

		fprintf(stderr, "exception caught:\n\t%s", e.what());
	}

	return to_sock;
}

//general send, include TCP & UDP
int generalSendTo(const char* host, int port, int to_sock, const char* buf,
		int bufsize, bool tcp) {

	int sentSize;

	if (tcp == true) {

//		sentSize = send(to_sock, (const void*) buf, bufsize, 0); // may try other flags......................

//		sentSize = sendDataBySize(to_sock, buf, bufsize);

		BdSendBase *pbsb = new BdSendToServer(buf);
		sentSize = pbsb->bsend(to_sock, tcp);

		delete pbsb;
		pbsb = NULL;

	} else { //UDP

		sentSize = udpSendTo(to_sock, host, port, buf, bufsize);
	}

	return sentSize;
}

ssize_t sendDataBySize(const int& to_sock, const char *byteBuf,
		const ssize_t& bytesToSend) {

	//const unsigned char lenbuf[4] = { ((bytesToSend >> 24) & 0xffu),
	const char lenbuf[4] = { ((bytesToSend >> 24) & 0xffu), ((bytesToSend >> 16)
			& 0xffu), ((bytesToSend >> 8) & 0xffu), (bytesToSend & 0xffu) };

	ssize_t ssize = 0;

	/*send data size*/
	ssize = sendWholeData(to_sock, lenbuf, 4);
	if (ssize < 4) {

		cerr << "net_util: error on sending size of data" << strerror(errno)
				<< endl;

		return -1;
	}

	/*send data*/
	ssize = sendWholeData(to_sock, byteBuf, bytesToSend);
	if (ssize < bytesToSend) {

		cerr << "net_util: error on sending data" << strerror(errno) << endl;

		return -1;
	}

	return ssize;
}

ssize_t sendWholeData(const int& to_sock, const char *byteBuf,
		const size_t& bytesToSend) {

	ssize_t bytesSent = 0;

	while (bytesSent < bytesToSend) {

		size_t toSend = bytesToSend - bytesSent;

		ssize_t sentNow = send(to_sock, &byteBuf[bytesSent], toSend, 0);

		if (sentNow != SOCKET_ERROR) {

			bytesSent += sentNow;

		} else {

			bytesSent = sentNow;
			//should handle this error somehow.
			break;
		}
	}

	return bytesSent;
}

//send back when you receive something
int generalSendBack(int to_sock, const char* buf, int bufsize,
		struct sockaddr_in sendbackAddr, int flag, bool tcp) {

	int sentSize = 0;
	if (tcp == true) {

//		sentSize = send(to_sock, (const void*) buf, bufsize, 0);

		BdSendBase *pbsb = new BdSendToClient(buf);
		sentSize = pbsb->bsend(to_sock, tcp);

		delete pbsb;
		pbsb = NULL;

	} else { //UDP

		sentSize = udpSendBack(to_sock, buf, bufsize, sendbackAddr, flag);
	}
	if (sentSize < 0) {
		cerr << "net_util: generalSendBack:  error on generalSendBack(): "
				<< strerror(errno) << endl;
		//return -1;
	}
	return sentSize;

}

int generalSendUDP(char* host, int port, int to_sock, char* buf, int bufsize) {

	/*
	 struct hostent *hp;
	 struct sockaddr_in server;
	 server.sin_family = AF_INET;
	 hp = gethostbyname(host);


	 bcopy((char *)hp->h_addr,
	 (char *)&server.sin_addr,
	 hp->h_length);
	 server.sin_port = htons(port);
	 int length = sizeof(struct sockaddr_in);
	 int n =sendto(to_sock,buf, strlen(buf),0,(const struct sockaddr *)&server,length);
	 */

	int sentSize;
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(struct sockaddr_in)); /*zero the struct*/
	struct hostent * hinfo = gethostbyname(host);
	if (hinfo == NULL)
		printf("getbyname failed!\n");
	dest.sin_family = AF_INET; /*storing the server info in sockaddr_in structure*/
	dest.sin_addr = *(struct in_addr *) (hinfo->h_addr); /*set destination IP number*/
	dest.sin_port = htons(port);
	sentSize = sendto(to_sock, (const void*) buf, bufsize, 0,
			(struct sockaddr*) &dest, sizeof(dest));
	if (sentSize != bufsize) {
		printf("generalSend:udp: send failed. error:");
		printf("%s\n", strerror(errno));
		close(to_sock);
		return -1;
	}
	return sentSize;
}

int loopedReceive(int sock, string& srecv, struct sockaddr_in & recvAddr,
		int flag, bool tcp) {

	ssize_t rsize = -2;

	BdRecvBase *pbrb = new BdRecvFromServer();

	while (1) {

		char buf[Env::BUF_SIZE];
		memset(buf, '\0', sizeof(buf));

		ssize_t count = recv(sock, buf, sizeof(buf), flag);

		if (count == -1 || count == 0) {

			rsize = count;

			break;
		}

		bool ready = false;

		string bd = pbrb->getBdStr(sock, buf, count, ready);

		if (ready) {

			srecv = bd;
			rsize = srecv.size();

			break;
		}

		memset(buf, '\0', sizeof(buf));
	}

	delete pbrb;
	pbrb = NULL;

	return rsize;
}

int generalReceive(int sock, void* buf, int bufsize,
		struct sockaddr_in & recvAddr, int flag, bool tcp) {

	int recvSize = 0;

	if (tcp == true) {

		recvSize = recv(sock, buf, bufsize, flag);

	} else { //UDP

		recvSize = udpRecvFrom(sock, buf, bufsize, recvAddr, flag);
	}

	if (recvSize < 0) {
		cerr << "net_util: generalReceive(): error on receive: "
				<< strerror(errno) << endl;
		return -1;
	}

	return recvSize;
}

int generalSendTCP(int to_sock, const char* buf, int bufsize) {

	int sentSize;

	sentSize = send(to_sock, (const void*) buf, bufsize, 0);

	if (sentSize < 0) {
		cerr << "net_util: error on generalSendTCP: " << strerror(errno)
				<< endl;
		return -1;
	}

	return sentSize;
}

int generalReveiveTCP(int sock, void *buf, size_t bufsize, int flags) {

	return recv(sock, buf, bufsize, flags);
}

ssize_t recvDataSize(const int& from_sock, size_t& bytesToRead) {

	/*receive size of data*/
//	unsigned char lenbuf[4];
	char lenbuf[4];
	ssize_t bytesRead = recvWholeData(from_sock, lenbuf, 4);

	bytesToRead = ((lenbuf[0] & 0xffu) << 24) | ((lenbuf[1] & 0xffu) << 16)
			| ((lenbuf[2] & 0xffu) << 8) | (lenbuf[3] & 0xffu);

	return bytesRead;
}

ssize_t recvDataBySize(const int& from_sock, char *byteBuf,
		size_t bytesToRead) {

	return recvWholeData(from_sock, byteBuf, bytesToRead);
}

ssize_t recvWholeData(const int& from_sock, char *byteBuf,
		const size_t& bytesToRead) {

	ssize_t bytesRead = 0;

	while (bytesRead < bytesToRead) {

		size_t toRead = bytesToRead - bytesRead;

		/*read as much as we can. note: byteInc may not == bytesToRead - bytesRead.*/
		ssize_t recvNow = recv(from_sock, &byteBuf[bytesRead], toRead, 0);

		if (recvNow != SOCKET_ERROR) {

			bytesRead += recvNow;

		} else {

			bytesRead = recvNow;
			//should handle this error somehow.
			break;
		}
	}

	return bytesRead;
	//buffer now contains the complete message.
//		some_processing_function(buffer);
}

int generalReceiveUDP(int sock, char* host, int port, void *buf, size_t bufsize,
		int flags, bool tcp) {
	unsigned int addrLen;

//Here I strongly adopt that this will work, I made a sockaddr_in with specified address: host and port
	struct sockaddr_in rcvFromAddr;
	int recvLen;
	memset(&rcvFromAddr, 0, sizeof(struct sockaddr_in)); /*zero the struct*/
	struct hostent * hinfo = gethostbyname(host);
	if (hinfo == NULL)
		printf("getbyname failed!\n");
	rcvFromAddr.sin_family = PF_INET; /*storing the server info in sockaddr_in structure*/
	rcvFromAddr.sin_addr = *(struct in_addr *) (hinfo->h_addr); /*set destination IP number*/
	rcvFromAddr.sin_port = htons(port);
//--------------------------------------------------
	addrLen = sizeof(rcvFromAddr);
	recvLen = recvfrom(sock, buf, bufsize, flags,
			(struct sockaddr*) &rcvFromAddr, &addrLen); //this cast from sockaddr_in to sockaddr could be wrong.
	if (recvLen < 0) {
		printf("net_util: generalReceiveUDP failed, error: %s\n",
				strerror(errno));
		close(sock);
		return -1;
	}
	return recvLen;
}

int simpleRecvUDP(int sock, void *buf, size_t bufsize, int flag,
		struct sockaddr_in *rcvFromAddr) {

	int recvLen = recvfrom(sock, buf, bufsize, flag,
			(struct sockaddr*) rcvFromAddr, NULL);
	return recvLen;
}

// toSock can be made by makeClientSock()
int udpSendTo(int toSock, const char* host, int port, const char* buf,
		int bufsize) {
	struct hostent *hp;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	hp = gethostbyname(host);

	bcopy((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
	server.sin_port = htons(port);
	int ret = sendto(toSock, buf, bufsize, 0, (struct sockaddr*) &server,
			sizeof(struct sockaddr));
	if (ret < 0) {
		cerr << "net_util.cpp: udpSendTo error: " << strerror(errno) << endl;
	}
	return ret;
}

int udpRecvFrom(int sock, void* buf, int bufsize, struct sockaddr_in & recvAddr,
		int flag) {

	socklen_t addr_len = sizeof(struct sockaddr);
	int ret = recvfrom(sock, buf, bufsize, flag, (struct sockaddr *) &recvAddr,
			&addr_len);
	if (ret < 0) {
		cerr << "net_util.cpp: udpRecvFrom error: " << strerror(errno) << endl;
	}
	return ret;
}

int udpSendBack(int sock, const char* buf, int bufsize,
		struct sockaddr_in sendbackAddr, int flag) {

	int ret = sendto(sock, buf, bufsize, flag,
			(struct sockaddr *) &sendbackAddr, sizeof(struct sockaddr));
//	cout<<"UDP sendback: ret =  "<<ret<<endl;
	if (ret < 0) {
		cerr << "net_util.cpp: udpSendBack error: " << strerror(errno) << endl;
	}
	return ret;

}

//may not work
int serverReceive(int sock, void *buf, size_t size, int flags, bool tcp) {
	if (tcp == true) {
		return recv(sock, buf, size, flags);
	} else { //---UDP
		unsigned int addrLen;
		struct sockaddr_in rcvFromAddr;
		int recvLen;
		memset(&rcvFromAddr, 0, sizeof(struct sockaddr_in)); /*zero the struct*/
		//--------------------------------------------------
		addrLen = sizeof(rcvFromAddr);
		recvLen = recvfrom(sock, buf, size, 0, (sockaddr *) &rcvFromAddr,
				&addrLen);
		if (recvLen < 0) {
			printf("d3_svr_recv_udp recvfrom failed, error: %s\n",
					strerror(errno));
			close(sock);
			return -1;
		}

		return recvLen;
	}
}

int reuseSock(int sock) {
	int reuse_addr = 1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	if (ret < 0) {
		cerr << "resuse socket failed: " << strerror(errno) << endl;
		return -1;
	} else
		return 0;
}
/*
 int setTCPLowLatency(int sock){
 int flag = 1;
 int ret = setsockopt( sock, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
 if (ret == -1) {
 printf("Couldn't setsockopt(TCP_NODELAY)\n");
 exit( EXIT_FAILURE );
 }
 return ret;
 }
 */
int setRecvTimeout(int sockfd, unsigned int sec, unsigned int usec) {
	struct timeval tv;

	tv.tv_sec = sec; /* 30 Secs Timeout */
	tv.tv_usec = 0; // Not init'ing this can cause strange errors

	return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv,
			sizeof(struct timeval));
}
