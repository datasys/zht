/*
 * mpi_proxy_stub.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MPI_PROXY_STUB_H_
#define MPI_PROXY_STUB_H_

#include "proxy_stub.h"

/*
 *
 */
class MPIProxy: public ProtoProxy {

public:
	MPIProxy();
	virtual ~MPIProxy();

	virtual bool init(int argc, char **argv);
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

	virtual bool init(int argc, char **argv);
	virtual bool recvsend(ProtoAddr addr, const void *recvbuf);

private:
	int size;
	int rank;
};
#endif /* MPI_PROXY_STUB_H_ */
