/*
 * ZHTServer.h
 *
 *  Created on: Jun 26, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef ZHTSERVER_H_
#define ZHTSERVER_H_

#include "ZProcessor.h"

using namespace iit::datasys::zht::dm;

/*
 *
 */
class ZHTServer: public ZProcessor {
public:
	ZHTServer();
	virtual ~ZHTServer();

	virtual void process(const int& fd, const char * const buf, sockaddr sender,
			const int& protocol);
};

#endif /* ZHTSERVER_H_ */
