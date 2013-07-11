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
 * mpi_broker.cpp
 *
 *  Created on: Jul 7, 2013
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include <errno.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "mq_proxy_stub.h"
#include "mpi_proxy_stub.h"

MQStub mqstub;
MPIProxy mpiproxy;

void sigint_handler(int sig) {

//	printf("sigint_handler called\n");

	mqstub.teardown();
}

void init_sig_handler() {

	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0; // or SA_RESTART

	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGINT, &sa, NULL) == -1) {

		perror("sigaction");
		exit(1);
	}
}

void init_me(void) {

	init_sig_handler();
}

int main(int argc, char **argv) {

	init_me();

	size_t msz;
	char req[IPC_MAX_MSG_SZ];
	char ans[IPC_MAX_MSG_SZ];

	mpiproxy.init(argc, argv);

	for (;;) {/* never quits! */

		if (!mqstub.recv(req, msz)) {

			perror("MQStub::recv()");
			exit(1);
		}

		if (!mpiproxy.sendrecv(req, msz, ans, msz)) {

			perror("MPIProxy::sendrecv()");
			exit(1);
		}

		if (!mqstub.send(ans, msz)) {

			perror("MQStub::send()");
			exit(1);
		}

		memset(req, 0, sizeof(req));
		memset(ans, 0, sizeof(ans));
	}

	return 0;
}
