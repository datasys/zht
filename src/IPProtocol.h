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
using namespace std;

/*
 *
 */
class IPProtocol: public Protocol {
public:
	IPProtocol();
	virtual ~IPProtocol();

protected:
	int reuseSock(int sock);
};

#endif /* IPPROTOCOL_H_ */
