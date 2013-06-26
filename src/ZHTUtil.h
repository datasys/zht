/*
 * ZHTUtil.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef ZHTUTIL_H_
#define ZHTUTIL_H_

#include <string>
#include <vector>
using namespace std;

#include <arpa/inet.h>

#include  "meta.pb.h"

struct HostEntity {

	struct sockaddr_in si;
	int sock;
	string host;
	int port;
	bool valid;
	vector<unsigned long long> ringID;
};

/*
 *
 */
class ZHTUtil {
public:
	ZHTUtil();
	virtual ~ZHTUtil();

	HostEntity getHostEntityByKey(const string& msg);

private:
	HostEntity buildHostEntity(const string& host, const uint& port);
};

class IdHelper {
public:
	IdHelper();
	virtual ~IdHelper();

	static uint64_t genId();

public:
	static const uint ID_LEN;
};

#endif /* ZHTUTIL_H_ */
