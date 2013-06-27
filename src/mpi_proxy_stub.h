/*
 * mpi_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MPI_PROXY_STUB_H_
#define MPI_PROXY_STUB_H_

#include "proxy_stub.h"

#include "ipc_plus.h"
using namespace IPC;

/*
 *
 */
class MPIProxy: public ProtoProxy {

private:
	MPIProxy();

public:
	MPIProxy(int argc, char **argv);
	virtual ~MPIProxy();

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

private:
	virtual int get_mpi_dest();

private:
	int size;
	int rank;
};

class MPIStub: public ProtoStub {

public:
	MPIStub();
	virtual ~MPIStub();

	virtual bool recvsend(void *recvbuf, size_t &recvcount, const void *sendbuf,
			const size_t sendcount);
};
#endif /* MPI_PROXY_STUB_H_ */
