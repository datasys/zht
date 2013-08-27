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
 * ip_proxy_stub.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "ip_proxy_stub.h"
#include "Env.h"
#include "bigdata_transfer.h"

#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <iostream>

using namespace std;

IPProtoProxy::IPProtoProxy() {
}

IPProtoProxy::~IPProtoProxy() {
}

int IPProtoProxy::reuseSock(int sock) {

	int reuse_addr = 1;
	int ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	if (ret < 0) {
		cerr << "resuse socket failed: [" << sock << "], " << strerror(errno)
				<< endl;
		return -1;
	} else
		return 0;
}

int IPProtoProxy::loopedrecv(int sock, void *senderAddr, string &srecv) {

	ssize_t recvcount = -2;
	socklen_t addr_len = sizeof(struct sockaddr);

	BdRecvBase *pbrb = new BdRecvFromServer();

	char buf[Env::BUF_SIZE];

	while (1) {

		memset(buf, '\0', sizeof(buf));

		ssize_t count;
		if (senderAddr == NULL)
			count = ::recv(sock, buf, sizeof(buf), 0);
		else
			count = ::recvfrom(sock, buf, sizeof(buf), 0,
					(struct sockaddr *) senderAddr, &addr_len);

		if (count == -1 || count == 0) {

			recvcount = count;

			break;
		}

		bool ready = false;

		string bd = pbrb->getBdStr(sock, buf, count, ready);

		if (ready) {

			srecv = bd;
			recvcount = srecv.size();

			break;
		}

		memset(buf, '\0', sizeof(buf));
	}

	delete pbrb;
	pbrb = NULL;

	return recvcount;
}

IPProtoStub::IPProtoStub() {
}

IPProtoStub::~IPProtoStub() {
}

