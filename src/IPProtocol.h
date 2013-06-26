/*
 * IPProtocol.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef IPPROTOCOL_H_
#define IPPROTOCOL_H_

#include "Protocol.h"

#include <string>
#include <sys/types.h>
using namespace std;

/*
 *
 */
class IPProtocol: public Protocol {
public:
	IPProtocol();
	virtual ~IPProtocol();

protected:
	virtual int reuseSock(int sock) = 0;
};

#endif /* IPPROTOCOL_H_ */
