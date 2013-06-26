/*
 * ZHTUtil.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#include "ZHTUtil.h"

#include "Util.h"
#include "ConfHandler.h"

#include <arpa/inet.h>
#include <algorithm>
#include <netdb.h>

using namespace iit::datasys::zht::dm;

ZHTUtil::ZHTUtil() {
}

ZHTUtil::~ZHTUtil() {
}

HostEntity ZHTUtil::getHostEntityByKey(const string& msg) {

	Package pkg;
	pkg.ParseFromString(msg);

	int index = HashUtil::genHash(pkg.virtualpath())
			% ConfHandler::NeighborVector.size();

	ConfEntry ce = ConfHandler::NeighborVector.at(index);

	return buildHostEntity(ce.name(), atoi(ce.value().c_str()));

}

HostEntity ZHTUtil::buildHostEntity(const string& host, const uint& port) {

	HostEntity he;

	struct sockaddr_in si_other;
	hostent *record;
	in_addr *address;
	string ip_address;

	record = gethostbyname(host.c_str());
	address = (in_addr *) record->h_addr;
	ip_address = inet_ntoa(*address);

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	if (inet_aton(ip_address.c_str(), &si_other.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
	}

	he.si = si_other;
	he.host = host;
	he.port = port;
	he.valid = true;
	he.sock = -1;

	return he;
}

const uint IdHelper::ID_LEN = 20;

IdHelper::IdHelper() {
}

IdHelper::~IdHelper() {
}

uint64_t IdHelper::genId() {

	return HashUtil::genHash(HashUtil::randomString(62).c_str());
}

