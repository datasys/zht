/*
 * Protocol.h
 *
 *  Created on: Jun 21, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <sys/types.h>

#include "protocol_shared.h"
/*
 *
 */
class Protocol {

public:
	Protocol();
	virtual ~Protocol();

	virtual bool send(const void *sendbuf, const size_t sendcount) = 0;

	virtual bool recv(void *recvbuf, size_t &recvcount) = 0;

	virtual bool sendrecv(const void *sendbuf, const size_t sendcount,
			void *recvbuf, size_t &recvcount) = 0;

	virtual bool teardown() = 0;
};

#endif /* PROTOCOL_H_ */
