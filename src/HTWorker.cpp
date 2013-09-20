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
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname Tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname Xiaobingo,
 *      Ke Wang(kwang22@hawk.iit.edu) with nickname KWang,
 *      Dongfang Zhao(dzhao8@@hawk.iit.edu) with nickname DZhao,
 *      Ioan Raicu(iraicu@cs.iit.edu).
 *
 * HTWorker.cpp
 *
 *  Created on: Sep 10, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "HTWorker.h"

#include "Const-impl.h"
#include "Env.h"
#include <unistd.h>
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

	ZPack zpack;
	string str(buf);
	zpack.ParseFromString(str);

	if (zpack.opcode() == Const::ZSC_OPC_LOOKUP) {

		result = lookup(zpack);
	} else if (zpack.opcode() == Const::ZSC_OPC_INSERT) {

		result = insert(zpack);
	} else if (zpack.opcode() == Const::ZSC_OPC_APPEND) {

		result = append(zpack);
	} else if (zpack.opcode() == Const::ZSC_OPC_CMPSWP) {

		result = compare_swap(zpack);
	} else if (zpack.opcode() == Const::ZSC_OPC_REMOVE) {

		result = remove(zpack);
	} else if (zpack.opcode() == Const::ZSC_OPC_STCHGCB) {

		result = state_change_callback(zpack);
	} else {

		result = Const::ZSC_REC_UOPC;
	}

	return result;
}

string HTWorker::insert(const ZPack &zpack) {

	string result;

	if (zpack.key().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = zpack.key();
	int ret = pmap->put(key, zpack.SerializeAsString());

	if (ret != 0) {

		cerr << "DB Error: fail to insert: rcode = " << ret << endl;
		result = Const::ZSC_REC_NONEXISTKEY; //-92

	} else {

		result = Const::ZSC_REC_SUCC; //0, succeed.
	}

	return result;
}

string HTWorker::lookup(const ZPack &zpack) {

	string result;

	if (zpack.key().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = zpack.key();
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

string HTWorker::append(const ZPack &zpack) {

	string result;

	if (zpack.key().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = zpack.key();
	int ret = pmap->append(key, zpack.SerializeAsString());

	if (ret != 0) {

		cerr << "DB Error: fail to append: rcode = " << ret << endl;
		result = Const::ZSC_REC_NONEXISTKEY; //-92

	} else {

		result = Const::ZSC_REC_SUCC; //0, succeed.
	}

	return result;
}

string HTWorker::state_change_callback(const ZPack &zpack) {

	string result;

	result = state_change_callback_internal(zpack);

	int poll_interval = Env::get_sccb_poll_interval();
	//printf("poll_interval: %d\n", poll_interval);

	while (result == Const::ZSC_REC_SCCBPOLLTRY) {

		usleep(poll_interval * 1000);

		result = state_change_callback_internal(zpack);
	}

	return result;
}

string HTWorker::state_change_callback_internal(const ZPack &zpack) {

	string result;

	if (zpack.key().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = zpack.key();
	string *ret = pmap->get(key);

	if (ret == NULL) {

		cerr << "DB Error: lookup find nothing" << endl;

		result = Const::ZSC_REC_NONEXISTKEY;

	} else {

		ZPack rltpack;
		rltpack.ParseFromString(*ret);

		if (zpack.val() == rltpack.val()) {

			result = Const::ZSC_REC_SUCC; //0, succeed.
		} else {

			result = Const::ZSC_REC_SCCBPOLLTRY;
		}
	}

	return result;
}

string HTWorker::compare_swap(const ZPack &zpack) {

	if (zpack.key().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string ret = compare_swap_internal(zpack);

	string result = lookup(zpack);

	ret.append(erase_status_code(result));

	return ret;
}

string HTWorker::compare_swap_internal(const ZPack &zpack) {

	string ret;

	/*get Package stored by lookup*/
	string lresult = lookup(zpack);
	ZPack lzpack;
	lresult = erase_status_code(lresult);
	lzpack.ParseFromString(lresult);

	string seen_value_pass_in = zpack.val();

	/*get seen_value stored*/
	string seen_value_stored = lzpack.val();

	/*	printf("{%s}:{%s,%s}\n", zpack.key().c_str(), zpack.val().c_str(),
	 zpack.newval().c_str());*/

	/*they are equivalent, compare and swap*/
	if (!seen_value_pass_in.compare(seen_value_stored)) {

		lzpack.set_val(zpack.newval());

		return insert(lzpack);

	} else {

		return Const::ZSC_REC_SRVEXP;
	}
}

string HTWorker::remove(const ZPack &zpack) {

	string result;

	if (zpack.key().empty())
		return Const::ZSC_REC_EMPTYKEY; //-1

	string key = zpack.key();
	int ret = pmap->remove(key);

	if (ret != 0) {

		cerr << "DB Error: fail to remove: rcode = " << ret << endl;
		result = Const::ZSC_REC_NONEXISTKEY; //-92

	} else {

		result = Const::ZSC_REC_SUCC; //0, succeed.
	}

	return result;
}

string HTWorker::erase_status_code(string & val) {

	return val.substr(3);
}
