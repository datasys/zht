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
 * mpi_proxy_stub.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "mpi_proxy_stub.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "mpi.h"
#include "HTWorker.h"

#include  "Util.h"
#include  "zpack.pb.h"
#include "ConfHandler.h"

using namespace iit::datasys::zht::dm;

MPIProxy::MPIProxy() :
		size(0), rank(-1) {
}

MPIProxy::~MPIProxy() {

	MPI_Finalize();
}

bool MPIProxy::init(int argc, char **argv) {

	srand(clock());

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//printf("spock[%d:%d]: ready to receive messages, captain.\n", size, rank);
}

bool MPIProxy::sendrecv(const void *sendbuf, const size_t sendcount,
		void *recvbuf, size_t &recvcount) {

	int mpi_dest = get_mpi_dest(sendbuf, sendcount);

	int rs = MPI_Send((void*) sendbuf, sendcount, MPI_CHAR, mpi_dest, 2,
			MPI_COMM_WORLD );

	//printf("spock, sent to [%d] %lu char(s): \"%s\"\n", mpi_dest, sendcount, (char*) sendbuf);

	size_t msz;
	char ans[IPC_MAX_MSG_SZ];

	MPI_Status status;
	int rr = MPI_Recv(ans, sizeof(ans), MPI_CHAR, mpi_dest, MPI_ANY_TAG,
			MPI_COMM_WORLD, &status);

	memcpy(recvbuf, ans, recvcount = strlen(ans) + 1);

	return rs == MPI_SUCCESS && rr == MPI_SUCCESS;

}

int MPIProxy::get_mpi_dest(const void *sendbuf, const size_t sendcount) {

	//todo: to know which ZHT server as dest, same as TCP_Proxy and UDP_Proxy

	ZPack zpack;
	zpack.ParsePartialFromArray(sendbuf, sendcount);

	uint64_t hascode = HashUtil::genHash(zpack.key());
	size_t node_size = ConfHandler::NeighborVector.size();
	int index = hascode % (size - node_size);

//	printf("[%lu][%lu][%d]\n", hascode, node_size, index);

	return index;

}

MPIStub::MPIStub() :
		size(0), rank(-1) {
}

bool MPIStub::init(int argc, char **argv) {

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}

MPIStub::~MPIStub() {

	MPI_Finalize();
}

bool MPIStub::recvsend(ProtoAddr addr, const void *recvbuf) {

	bool rr_bool;
	bool rs_bool;

	MPI_Status status;

	int again = 0;
	char req[IPC_MAX_MSG_SZ];

	fprintf(stdout, "[%d,%d] mpi server loop %d\n", size, rank, again);

	for (;;) {

		++again;

		int rr = MPI_Recv(req, sizeof(req), MPI_CHAR, MPI_ANY_SOURCE,
				MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		/*get response to be sent to client*/
		HTWorker htw;
		string result = htw.run((char*) req);

		const char *sendbuf = result.data();
		int sendcount = result.size();

		/*send response to client over MPI_SOURCE*/
		int rs = MPI_Send((void*) sendbuf, sendcount, MPI_CHAR,
				status.MPI_SOURCE, 0, MPI_COMM_WORLD );

		memset(req, 0, sizeof(req));

		rr_bool = rr == MPI_SUCCESS;
		rs_bool = rs == MPI_SUCCESS;
	}

	return rr_bool && rs_bool;
}

/*
 bool MPIStub::recvsend(void *recvbuf, size_t &recvcount, const void *sendbuf,
 const size_t sendcount) {

 bool rr_bool;
 bool rs_bool;

 MPI_Status status;

 int again = 0;

 size_t msz;
 char req[IPC_MAX_MSG_SZ];
 char ans[IPC_MAX_MSG_SZ];

 for (;;) {

 ++again;

 fprintf(stderr, "[%d] mpi server loop %d\n", rank, again);

 int rr = MPI_Recv(req, sizeof(req), MPI_CHAR, MPI_ANY_SOURCE,
 MPI_ANY_TAG, MPI_COMM_WORLD, &status);

 do something
 fprintf(stderr, "[%d] do something: [%lu] [%s]\n", rank, strlen(req),
 req);
 strcat(req, " [answered]");
 strncpy(ans, req, strlen(req) + 1);

 int rs = MPI_Send(ans, strlen(ans) + 1, MPI_CHAR, status.MPI_SOURCE, 0,
 MPI_COMM_WORLD );

 memset(req, 0, sizeof(req));
 memset(ans, 0, sizeof(ans));

 rr_bool = rr == MPI_SUCCESS;
 rs_bool = rs == MPI_SUCCESS;
 }

 return rr_bool && rs_bool;
 }*/
