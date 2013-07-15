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
 * Env.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "Env.h"

//const int Env::MAX_MSG_SIZE = 1024 * 2; //max size of a message in each transfer
//const int Env::MAX_MSG_SIZE = 65535; //max size of a message in each transfer

const int Env::MAX_MSG_SIZE = 1024 * 10000 * 7; //max size of a message in each transfer

const uint Env::BUF_SIZE = 512 + 38;

const int Env::TOTAL_MSG_SIZE = 1024 * 1024 * 10; //total size of a message transfered, 10M

int Env::REPLICATION_TYPE = 0; //1 for Client-side replication

int Env::NUM_REPLICAS = 0;

Env::Env() {
}

Env::~Env() {
}
