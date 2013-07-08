/*
 * ProxyStubFactory.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#include "ProxyStubFactory.h"

#ifdef PF_INET
#include  "tcp_proxy_stub.h"
#include  "udp_proxy_stub.h"
#include  "mq_proxy_stub.h"
#elif MPI_INET
#include  "mpi_proxy_stub.h"
#include  "mq_proxy_stub.h"
#endif

#include  "ConfHandler.h"
using namespace iit::datasys::zht::dm;

ProxyStubFactory::ProxyStubFactory() {

}

ProxyStubFactory::~ProxyStubFactory() {
}

ProtoProxy* ProxyStubFactory::createProxy() {

	ConfHandler::MAP *zpmap = &ConfHandler::ZHTParameters;

	ConfEntry ce_tcp(Const::PROTO_NAME, Const::PROTO_VAL_TCP); //TCP
	ConfEntry ce_udp(Const::PROTO_NAME, Const::PROTO_VAL_UDP); //UDP
	ConfEntry ce_mpi(Const::PROTO_NAME, Const::PROTO_VAL_MPI); //MPI

#ifdef PF_INET

	if (zpmap->find(ce_tcp.toString()) != zpmap->end())
	return new TCPProxy();

	if (zpmap->find(ce_udp.toString()) != zpmap->end())
	return new UDPProxy();

	if (zpmap->find(ce_mpi.toString()) != zpmap->end())
	return new MQProxy();

#endif

	return 0;
}

ProtoStub* ProxyStubFactory::createStub() {

	ConfHandler::MAP *zpmap = &ConfHandler::ZHTParameters;

	ConfEntry ce_tcp(Const::PROTO_NAME, Const::PROTO_VAL_TCP); //TCP
	ConfEntry ce_udp(Const::PROTO_NAME, Const::PROTO_VAL_UDP); //UDP
	ConfEntry ce_mpi(Const::PROTO_NAME, Const::PROTO_VAL_MPI); //MPI

#ifdef PF_INET

	if (zpmap->find(ce_tcp.toString()) != zpmap->end())
	return new TCPStub();

	if (zpmap->find(ce_udp.toString()) != zpmap->end())
	return new UDPStub();
#elif MPI_INET

	if (zpmap->find(ce_mpi.toString()) != zpmap->end())
	return new MPIStub();
#endif

	return 0;
}

