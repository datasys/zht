/*
 * mpi_server.h
 *
 *  Created on: Jul 5, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MPISERVER_H_
#define MPISERVER_H_

/*
 *
 */
class MPIServer {
public:
	MPIServer();
	virtual ~MPIServer();

private:
	void init();

private:
	MPI_Status status;

	int size;
	int rank;

	size_t msz;
	/*char req[IPC_MAX_MSG_SZ];
	 char ans[IPC_MAX_MSG_SZ];*/
};

#endif /* MPISERVER_H_ */
