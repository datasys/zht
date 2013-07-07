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
 * HTWorker.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: tony, xiaobingo
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
