/*
 * Copyright (C) 2010-2020 DatasysLab@iit.edu(http://datasys.cs.iit.edu/index.html).
 *      Director: Ioan Raicu(iraicu@cs.iit.edu)
 *	 
 * This file is part of ZHT library(http://datasys.cs.iit.edu/projects/ZHT/index.html).
 *      Ioan Raicu(iraicu@cs.iit.edu),
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname xiaobingo.
 * 
 * The ZHT Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The ZHT Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the ZHT Library; if not, write to the 
 * Data-Intensive Distributed Systems Laboratory, 10 W. 31st Street,
 * Stuart Building, Room 003B, Chicago, IL 60616 USA.
 *
 * EpollServer.h
 *
 *  Created on: Aug 9, 2012
 *      Author: tony, xiaobingo
 */

#ifndef EPOLLSERVER_H_
#define EPOLLSERVER_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ZProcessor.h"

#include "bigdata_transfer.h"

namespace iit {
namespace datasys {
namespace zht {
namespace dm {

class EpollData {
public:
	EpollData(const int& fd, const sockaddr * const sender);
	virtual ~EpollData();

	int fd() const;
	const sockaddr * const sender() const;

private:
	int _fd;
	const sockaddr * const _sender;
};
/*
 *
 */
class EpollServer {
public:
	EpollServer(ZProcessor *processor, bool tcp = true);
	virtual ~EpollServer();

	int serve(const char *port);

private:
	int create_and_bind(const char *port);
	int create_and_bind(const char *host, const char *port);
	int make_socket_non_blocking(const int& sfd);
	int makeSvrSocket(int port);
	int reuseSock(int sock);

private:
	EpollServer();
	bool _tcp;
	static const int MAX_EVENTS;
	static const uint MAX_MSG_SIZE;
	ZProcessor *_ZProcessor;
	BdRecvBase *pbrb;

};

} /* namespace dm */
} /* namespace zht */
} /* namespace datasys */
} /* namespace iit */
#endif /* EPOLLSERVER_H_ */
