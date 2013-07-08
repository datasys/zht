/*
 * mpi_server.cpp
 *
 *  Created on: Jul 5, 2013
 *      Author: Xiaobing Zhou
 */

#include "mpi_server.h"
#include "ProxyStubFactory.h"

MPIServer::MPIServer(int argc, char **argv) :
		_argc(argc), _argv(argv) {
}

MPIServer::~MPIServer() {
}

void MPIServer::serve() {

	ProtoStub *stub = ProxyStubFactory::createStub();
	stub->init(_argc, _argv);

	ProtoAddr pa;
	stub->recvsend(pa, 0);
}

