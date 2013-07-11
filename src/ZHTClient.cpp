/*
 * Copyright 2010-2020 DatasysLab@iit.edu(http://datasys.cs.iit.edu/index.html)
 *      Director: Ioan Raicu(iraicu@cs.iit.edu)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This file is part of ZHT library(http://datasys.cs.iit.edu/projects/ZHT/index.html).
 *      Ioan Raicu(iraicu@cs.iit.edu),
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname Tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname Xiaobingo.
 *
 * ZHTClient.cpp
 *
 *  Created on: Sep 16, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony
 */

#include "ZHTClient.h"

#include "meta.pb.h"
#include "ConfHandler.h"
#include "Env.h"

using namespace iit::datasys::zht::dm;

ZHTClient::ZHTClient() :
		_proxy(0) {

}

ZHTClient::ZHTClient(const string& zht_conf, const string& neighbor_conf) {

	init(zht_conf, neighbor_conf);
}

ZHTClient::~ZHTClient() {

	if (_proxy != NULL) {

		delete _proxy;
		_proxy = NULL;
	}
}

int ZHTClient::init(const string& zhtConf, const string& neighborConf) {

	ConfHandler::initConf(zhtConf, neighborConf);

	_proxy = ProxyStubFactory::createProxy();

	if (_proxy == 0)
		return -1;
	else
		return 0;
}

int ZHTClient::commonOp(const string& opcode, const string& pair,
		string& result) {

	if (opcode != Const::ZSC_OPC_LOOKUP && opcode != Const::ZSC_OPC_REMOVE
			&& opcode != Const::ZSC_OPC_INSERT
			&& opcode != Const::ZSC_OPC_APPEND)
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

int ZHTClient::append(const string& pair) {

	string result;
	return commonOp(Const::ZSC_OPC_APPEND, pair, result);
}

string ZHTClient::commonOpInternal(const string& opcode, const string& pair,
		string& result) {

	Package pkg;
	pkg.ParseFromString(pair);
	pkg.set_opcode(opcode); //"001": lookup, "002": remove, "003": insert, "004": append
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
	_proxy->sendrecv(msg.c_str(), msg.size(), buf, msz);

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

/*

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
 */

int ZHTClient::teardown() {

	if (_proxy->teardown())
		return 0;
	else
		return -1;
}
