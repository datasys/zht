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

private:
	MPIServer();

public:
	MPIServer(int argc, char **argv);

	virtual ~MPIServer();

	void serve();

private:
	int _argc;
	char ** _argv;
};

#endif /* MPISERVER_H_ */
