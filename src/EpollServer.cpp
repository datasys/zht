/*
 * Copyright 2010-2020 DatasysLab@iit.edu(http://datasys.cs.iit.edu/index.html)
 *      Director: Ioan Raicu(iraicu@cs.iit.edu)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is part of ZHT library(http://datasys.cs.iit.edu/projects/ZHT/index.html).
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname Tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname Xiaobingo,
 *      Ke Wang(kwang22@hawk.iit.edu) with nickname KWang,
 *      Dongfang Zhao(dzhao8@@hawk.iit.edu) with nickname DZhao,
 *      Ioan Raicu(iraicu@cs.iit.edu).
 *
 * EpollServer.cpp
 *
 *  Created on: Aug 9, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "EpollServer.h"

#include "Env.h"
#include "Const-impl.h"
#include "ConfHandler.h"

#include <stdlib.h>
#include <sys/epoll.h>

#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

using namespace std;

namespace iit {
namespace datasys {
namespace zht {
namespace dm {

EpollData::EpollData(const int& fd, const sockaddr * const sender) :
		_fd(fd), _sender(sender) {

}

EpollData::~EpollData() {

	if (_sender != NULL)
		delete _sender;
}

const int EpollServer::MAX_EVENTS = 1024;
const uint EpollServer::MAX_MSG_SIZE = Env::BUF_SIZE; //transferd string maximum size

EpollServer::EpollServer(const char *port, ZProcessor *processor) :
		_port(port), _ZProcessor(processor), pbrb(new BdRecvFromClient()) {

	string protocol = ConfHandler::getProtocolFromConf();
	if (protocol == Const::PROTO_VAL_TCP) {

		_tcp = true;
	} else if (protocol == Const::PROTO_VAL_UDP) {

		_tcp = false;
	} else {

		fprintf(stderr,
				"EpollServer::EpollServer(): <%s>, unrecognized IP family protocol\n",
				protocol.c_str());
		exit(1);
	}
}

EpollServer::~EpollServer() {

	if (_ZProcessor != NULL) {

		delete _ZProcessor;
		_ZProcessor == NULL;
	}
}

int EpollData::fd() const {
	return _fd;
}

const sockaddr * const EpollData::sender() const {
	return _sender;
}

int EpollServer::create_and_bind(const char *port) {

	return create_and_bind(NULL, port);
}

int EpollServer::create_and_bind(const char *host, const char *port) {

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE; /* All interfaces */

	s = getaddrinfo(host, port, &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {

		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0) {
			/* We managed to bind successfully! */
			break;
		}

		close(sfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not bind\n");
		return -1;
	}

	freeaddrinfo(result);

	return sfd;
}

int EpollServer::make_socket_non_blocking(const int& sfd) {

	int flags, s;

	flags = fcntl(sfd, F_GETFL, 0);
	if (flags == -1) {
		perror("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl(sfd, F_SETFL, flags);
	if (s == -1) {
		perror("fcntl");
		return -1;
	}

	return 0;
}

int EpollServer::makeSvrSocket() { //only for svr

	int port = atoi(_port);
	struct sockaddr_in svrAdd_in; /* socket info about our server */
	int svrSock = -1;

	try {

		memset(&svrAdd_in, 0, sizeof(struct sockaddr_in)); /* zero the struct before filling the fields */
		svrAdd_in.sin_family = AF_INET; /* set the type of connection to TCP/IP */
		svrAdd_in.sin_addr.s_addr = INADDR_ANY; /* set our address to any interface */
		svrAdd_in.sin_port = htons(port); /* set the server port number */

		if (_tcp == true) { //make socket

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

		if (bind(svrSock, (struct sockaddr*) &svrAdd_in,
				sizeof(struct sockaddr)) < 0) {

			printf(
					"Error occurred binding the socket:%d to the server port:%d\n",
					svrSock, port);
			printf("%s", strerror(errno));

			close(svrSock);
			return -1;
		}

		if (_tcp == true) { //TCP needs listen, UDP does not.

			/* start listening, allowing a queue of up to 1 pending connection */
			if (listen(svrSock, SOMAXCONN) < 0) {

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

int EpollServer::reuseSock(int sock) {

	int reuse_addr = 1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	if (ret < 0) {
		cerr << "resuse socket failed: " << strerror(errno) << endl;
		return -1;
	} else
		return 0;
}

void EpollServer::serve() {

	int sfd, s;
	int efd;
	struct epoll_event event;
	struct epoll_event *events;

	sfd = makeSvrSocket();
	if (sfd == -1)
		abort();

	s = make_socket_non_blocking(sfd);
	if (s == -1)
		abort();

	reuseSock(sfd);

	efd = epoll_create(1);
	if (efd == -1) {
		perror("epoll_create");
		abort();
	}

	event.data.ptr = new EpollData(sfd, NULL);
	event.events = EPOLLIN | EPOLLET;
	s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
	if (s == -1) {
		perror("epoll_ctl");
		abort();
	}

	/* Buffer where events are returned */
	events = (epoll_event *) calloc(MAX_EVENTS, sizeof event);

	/* The event loop */
	while (1) {

		int n, i;

		n = epoll_wait(efd, events, MAX_EVENTS, -1);

		for (i = 0; i < n; i++) {

			EpollData *edata = (EpollData*) events[i].data.ptr;

			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
					|| (!(events[i].events & EPOLLIN))) {

				/* An error has occured on this fd, or the socket is not
				 ready for reading (why were we notified then?) */
				fprintf(stderr, "epoll error\n");
				close(edata->fd());
				delete edata;
				continue;
			} else if (sfd == edata->fd()) {

				if (_tcp == true) {
					/* We have a notification on the listening socket, which
					 means one or more incoming connections. */
					while (1) {

						int infd;
						socklen_t in_len = sizeof(struct sockaddr);
						sockaddr *in_addr = (sockaddr *) calloc(1,
								sizeof(struct sockaddr));

						infd = accept(sfd, in_addr, &in_len);
						if (infd == -1) {

							free(in_addr);

							if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {

								/* We have processed all incoming
								 connections. */
								break;
							} else {

								perror("accept");
								break;
							}
						}

						/* fprintf(stdout,
						 "sin_family[%hu], sin_zero[%s], sin_addr.s_addr[%u], sin_port[%hu]\n",
						 in_addr.sin_family, in_addr.sin_zero,
						 in_addr.sin_addr.s_addr, in_addr.sin_port);
						 */

						char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
						getnameinfo(in_addr, in_len, hbuf, sizeof hbuf, sbuf,
								sizeof sbuf, 0);

						/*if (s == 0) {

						 printf("Accepted connection on descriptor %d "
						 "(host=%s, _port=%s)\n", infd, hbuf, sbuf);
						 }*/

						/* Make the incoming socket non-blocking and add it to the
						 list of fds to monitor. */
						s = make_socket_non_blocking(infd);
						if (s == -1) {

							free(in_addr);
							abort();
						}

						reuseSock(infd);

						event.data.ptr = new EpollData(infd, in_addr);
						event.events = EPOLLIN | EPOLLET;
						s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
						if (s == -1) {

							free(in_addr);
							perror("epoll_ctl");
							abort();
						}
					}
					continue;
				} else {

					ssize_t count;
					char buf[MAX_MSG_SIZE];
					memset(buf, 0, sizeof(buf));

					sockaddr_in fromaddr;
					socklen_t sender_len = sizeof(struct sockaddr);
					count = recvfrom(edata->fd(), buf, sizeof buf, 0,
							(struct sockaddr*) &fromaddr, &sender_len);

					sockaddr *sender = (struct sockaddr*) &fromaddr;

					string str(buf);
					_ZProcessor->process(edata->fd(), buf, *sender); //todo: problem maybe

					memset(buf, 0, sizeof(buf));
				}

			} else {

				if (_tcp == true) {
					/* We have data on the fd waiting to be read. Read and
					 display it. We must read whatever data is available
					 completely, as we are running in edge-triggered mode
					 and won't get a notification again for the same
					 data. */
					int done = 0;

					while (1) {

						ssize_t count;
						char buf[MAX_MSG_SIZE];
						memset(buf, 0, sizeof(buf));

						count = recv(edata->fd(), buf, sizeof(buf), 0);

						if (count == -1) {

							/* If errno == EAGAIN, that means we have read all
							 data. So go back to the main loop. */
							if (errno != EAGAIN) {

								perror("read");
								done = 1;
							} /*else {

							 printf(
							 "Closed connection on descriptor %d, -1<--recv\n",
							 edata->fd());

							 close(edata->fd());
							 delete edata;
							 }*/
							break;
						} else if (count == 0) {

							/* End of file. The remote has closed the
							 connection. */
							done = 1;
							break;
						} else {

							bool ready = false;
							string bd = pbrb->getBdStr(sfd, buf, count, ready);

							if (ready) {

								_ZProcessor->process(edata->fd(), bd.c_str(),
										*edata->sender());
							}
						}

						memset(buf, 0, sizeof(buf));
					}
					if (done) {

						/*printf("Closed connection on descriptor %d, done.\n",
						 edata->fd());*/

						/* Closing the descriptor will make epoll remove it
						 from the set of descriptors which are monitored. */
						close(edata->fd());
						delete edata;
					}
				} //if TCP == true
			}
		}
	}

	free(events);

	close(sfd);

	EpollData *gedata = (EpollData*) event.data.ptr;
	delete gedata;
}

} /* namespace dm */
} /* namespace zht */
} /* namespace datasys */
} /* namespace iit */
