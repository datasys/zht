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
 * HTWorker.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony
 */

#ifndef HTWORKER_H_
#define HTWORKER_H_

#include "meta.pb.h"
#include "novoht.h"

#include <string>
using namespace std;

/*
 *
 */
class HTWorker {
public:
	HTWorker();
	virtual ~HTWorker();

public:
	string run(const char *buf);

private:
	string lookup(const Package &pkg);
	string remove(const Package &pkg);
	string insert(const Package &pkg);
	string append(const Package &pkg);

private:
	static NoVoHT *pmap;
};

#endif /* HTWORKER_H_ */
