/*
 * ip_server.h
 *
 *  Created on: Jul 5, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef IPSERVER_H_
#define IPSERVER_H_

#include "ZProcessor.h"

using namespace iit::datasys::zht::dm;

/*
 *
 */
class IPServer: public ZProcessor {
public:
	IPServer();
	virtual ~IPServer();

	virtual void process(const int& fd, const char * const buf,
			sockaddr sender);
};

#endif /* IPSERVER_H_ */
