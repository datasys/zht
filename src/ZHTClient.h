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
 * ZHTClient.h
 *
 *  Created on: Sep 16, 2012
 *      Author: Tony
 *      Contributor: Xiaobingo
 */

#ifndef ZHTCLIENT_H_
#define ZHTCLIENT_H_

#include <stdint.h>
#include <map>
#include <string>
using namespace std;

#include "lru_cache.h"

#include "ProxyStubFactory.h"

namespace iit {
namespace datasys {
namespace zht {
namespace dm {

/*
 *
 */
class ZHTClient {

public:
	ZHTClient();

	ZHTClient(const string& zht_conf, const string& neighbor_conf);
	virtual ~ZHTClient();

	int init(const string& zht_conf, const string& neighbor_conf);
	int lookup(const string& pair, string& result);
	int remove(const string& pair);
	int insert(const string& pair);
	int append(const string& pair);
	int teardown();

private:
	int commonOp(const string& opcode, const string& pair, string& result);
	string commonOpInternal(const string& opcode, const string& pair,
			string& result);

private:
	ProtoProxy *_proxy;
};

} /* namespace dm */
} /* namespace zht */
} /* namespace datasys */
} /* namespace iit */
#endif /* ZHTCLIENT_H_ */
