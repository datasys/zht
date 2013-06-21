/*
 * MPIProxy.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MPIPROXY_H_
#define MPIPROXY_H_

#include "ProtocolProxy.h"
/*
 *
 */
class MPIProxy: public ProtocolProxy {
public:
	MPIProxy();
	virtual ~MPIProxy();

	virtual int sendMsg(const MsgDest &msq_dest, const void * msgp,
			size_t msgsz);

	virtual int sendMsg(const MsgDest &msq_dest, const void * msgp,
			size_t msgsz, int msgflg);
};

#endif /* MPIPROXY_H_ */
