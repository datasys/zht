/*
 * Copyright (C) 2010-2020 DatasysLab@iit.edu(http://datasys.cs.iit.edu/index.html).
 *      Director: Ioan Raicu(iraicu@cs.iit.edu)
 *	 
 * This file is part of ZHT library(http://datasys.cs.iit.edu/projects/ZHT/index.html).
 *      Ioan Raicu(iraicu@cs.iit.edu),
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname xiaobingo.
 * 
 * The ZHT Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The ZHT Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the ZHT Library; if not, write to the 
 * Data-Intensive Distributed Systems Laboratory, 10 W. 31st Street,
 * Stuart Building, Room 003B, Chicago, IL 60616 USA.
 *
 * ZHTClient.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: tony, xiaobingo
 */

#include "ZHTClient.h"

#include "meta.pb.h"
#include "ConfHandler.h"
#include "Env.h"

namespace iit {
namespace datasys {
namespace zht {
namespace dm {

ZHTClient::ZHTClient() :
		proxy(0) {

}

ZHTClient::ZHTClient(const string& zht_conf, const string& neighbor_conf) {

	init(zht_conf, neighbor_conf);
}

ZHTClient::~ZHTClient() {

	if (proxy != NULL) {

		delete proxy;
		proxy = NULL;
	}
}

int ZHTClient::init(const string& zht_conf, const string& neighbor_conf) {

	ConfHandler::CONF_ZHT = zht_conf;
	ConfHandler::CONF_NEIGHBOR = neighbor_conf;

	ConfHandler::setZHTParameters(zht_conf);
	ConfHandler::setNeighborSeeds(neighbor_conf);

	proxy = ProxyStubFactory::createProxy();

	return 0; //todo: take care the return code
}

int ZHTClient::commonOp(const string& opcode, const string& pair,
		string& result) {

	if (opcode != Const::ZSC_OPC_LOOKUP && opcode != Const::ZSC_OPC_REMOVE
			&& opcode != Const::ZSC_OPC_INSERT)
		return Const::toInt(Const::ZSC_REC_UOPC);

	string sstatus = commonOpInternal(opcode, pair, result);

	int status = Const::ZSI_REC_CLTFAIL;
	if (!sstatus.empty())
		status = Const::toInt(sstatus);

	return status;
}

int ZHTClient::lookup(const string& pair, string& result) {

	return commonOp(Const::ZSC_OPC_LOOKUP, pair, result);
}

int ZHTClient::remove(const string& pair) {

	string result;
	return commonOp(Const::ZSC_OPC_REMOVE, pair, result);
}

int ZHTClient::insert(const string& pair) {

	string result;
	return commonOp(Const::ZSC_OPC_INSERT, pair, result);
}

string ZHTClient::commonOpInternal(const string& opcode, const string& pair,
		string& result) {

	Package pkg;
	pkg.ParseFromString(pair);
	pkg.set_opcode(opcode); //"001": lookup, "002": remove, "003": insert
	pkg.set_replicano(3);

	if (pkg.virtualpath().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1, empty key not allowed.

	if (pkg.realfullpath().empty())
		pkg.set_realfullpath(" "); //coup, to fix ridiculous bug of protobuf!

	string msg = pkg.SerializeAsString();

	char buf[Env::MAX_MSG_SIZE];
	memset(buf, 0, sizeof(buf));

	size_t msz = sizeof(buf);

	/*send to and receive from*/
	proxy->sendrecv(msg.c_str(), msg.size(), buf, msz);

	/*...parse status and result*/
	string sstatus;

	string srecv(buf);

	if (srecv.empty()) {

		sstatus = Const::ZSC_REC_SRVEXP;
	} else {

		result = srecv.substr(3); //the left, if any, is lookup result or second-try pkg
		sstatus = srecv.substr(0, 3); //status returned, the first three chars, like 001, -98...
	}

	return sstatus;
}

void ZHTClient::parseStatusAndResult(string& sstatus, string& result) {

	size_t msz;
	char buf[Env::MAX_MSG_SIZE];
	memset(buf, 0, sizeof(buf));

	proxy->recv(buf, msz);

	string srecv;
	srecv.assign(buf);

	if (srecv.empty()) {

		sstatus = Const::ZSC_REC_SRVEXP;
	} else {

		result = srecv.substr(3); //the left, if any, is lookup result or second-try pkg
		sstatus = srecv.substr(0, 3); //status returned, the first three chars, like 001, -98...
	}
}

int ZHTClient::tearDown() {

	if (proxy->teardown())
		return 0;
	else
		return -1;
}

} /* namespace dm */
} /* namespace zht */
} /* namespace datasys */
} /* namespace iit */
