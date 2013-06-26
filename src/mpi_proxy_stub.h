/*
 * mpi_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MPI_PROXY_STUB_H_
#define MPI_PROXY_STUB_H_

#include "Protocol.h"

#include "ipc_plus.h"
using namespace IPC;

/*
 *
 */
class MPIProxy: public Protocol {

private:
	MPIProxy();

public:
	MPIProxy(int argc, char **argv);
	virtual ~MPIProxy();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();

private:
	virtual int get_mpi_dest();

private:
	int size;
	int rank;
};

class MPIStub: public Protocol {

public:
	MPIStub();
	virtual ~MPIStub();

	virtual bool send(const void *sendbuf, const size_t sendcount);

	virtual bool recv(void *recvbuf, size_t &recvcount);

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount);

	virtual bool teardown();
};
#endif /* MPI_PROXY_STUB_H_ */
