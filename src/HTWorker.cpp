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
 * HTWorker.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony
 */

#include "HTWorker.h"

#include "Const-impl.h"

#include <iostream>

using namespace std;
using namespace iit::datasys::zht::dm;

NoVoHT* HTWorker::pmap = new NoVoHT("", 100000, 10000, 0.7);

HTWorker::HTWorker() {
}

HTWorker::~HTWorker() {
}

string HTWorker::run(const char *buf) {

	string result;

	Package pkg;
	string str(buf);
	pkg.ParseFromString(str);

	if (pkg.opcode() == Const::ZSC_OPC_LOOKUP) {

		result = lookup(pkg);

	} else if (pkg.opcode() == Const::ZSC_OPC_REMOVE) {

		result = remove(pkg);

	} else if (pkg.opcode() == Const::ZSC_OPC_INSERT) {

		result = insert(pkg);

	} else if (pkg.opcode() == Const::ZSC_OPC_APPEND) {

		result = append(pkg);

	} else {

		result = Const::ZSC_REC_UOPC;
	}

	return result;
}

string HTWorker::lookup(const Package &pkg) {

	string result;

	if (pkg.virtualpath().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = pkg.virtualpath();
	string *ret = pmap->get(key);

	if (ret == NULL) {

		cerr << "DB Error: lookup find nothing" << endl;

		result = Const::ZSC_REC_NONEXISTKEY;
		result.append("Empty");

	} else {

		result = Const::ZSC_REC_SUCC;
		result.append(*ret);
	}

	return result;
}

string HTWorker::remove(const Package &pkg) {

	string result;

	if (pkg.virtualpath().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = pkg.virtualpath();
	int ret = pmap->remove(key);

	if (ret != 0) {

		cerr << "DB Error: fail to remove: rcode = " << ret << endl;
		result = Const::ZSC_REC_NONEXISTKEY; //-92

	} else {

		result = Const::ZSC_REC_SUCC; //0, succeed.
	}

	return result;
}

string HTWorker::insert(const Package &pkg) {

	string result;

	if (pkg.virtualpath().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = pkg.virtualpath();
	int ret = pmap->put(key, pkg.SerializeAsString());

	if (ret != 0) {

		cerr << "DB Error: fail to insert: rcode = " << ret << endl;
		result = Const::ZSC_REC_NONEXISTKEY; //-92

	} else {

		result = Const::ZSC_REC_SUCC; //0, succeed.
	}

	return result;
}

string HTWorker::append(const Package &pkg) {

	string result;

	if (pkg.virtualpath().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = pkg.virtualpath();
	int ret = pmap->append(key, pkg.SerializeAsString());

	if (ret != 0) {

		cerr << "DB Error: fail to append: rcode = " << ret << endl;
		result = Const::ZSC_REC_NONEXISTKEY; //-92

	} else {

		result = Const::ZSC_REC_SUCC; //0, succeed.
	}

	return result;
}
