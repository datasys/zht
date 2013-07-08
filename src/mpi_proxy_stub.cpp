/*
 * mpi_proxy_stub.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "mpi_proxy_stub.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "mpi.h"
#include "HTWorker.h"

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

	int mpi_dest = get_mpi_dest();

	int rs = MPI_Send(sendbuf, sendcount, MPI_CHAR, mpi_dest, 2,
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

int MPIProxy::get_mpi_dest() {

	//todo: to know which ZHT server as dest, same as TCP_Proxy and UDP_Proxy
	return rand() % (size - 1);
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

	for (;;) {

		++again;

		fprintf(stderr, "[%d] mpi server loop %d\n", rank, again);

		int rr = MPI_Recv(req, sizeof(req), MPI_CHAR, MPI_ANY_SOURCE,
				MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		/*get response to be sent to client*/
		HTWorker htw;
		string result = htw.run((char*) req);

		const char *sendbuf = result.data();
		int sendcount = result.size();

		/*send response to client over MPI_SOURCE*/
		int rs = MPI_Send(sendbuf, sendcount, MPI_CHAR, status.MPI_SOURCE, 0,
				MPI_COMM_WORLD );

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
