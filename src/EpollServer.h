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
 * EpollServer.h
 *
 *  Created on: Aug 9, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
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
	EpollServer(const char *port, ZProcessor *processor);
	virtual ~EpollServer();

	void serve();

private:
	int create_and_bind(const char *port);
	int create_and_bind(const char *host, const char *port);
	int make_socket_non_blocking(const int& sfd);
	int makeSvrSocket();
	int reuseSock(int sock);

private:
	EpollServer();

private:
	const char *_port;
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
