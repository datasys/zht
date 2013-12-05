/*
 * Copyright 2013-2020
 *      Advisor: Zhiling Lan(lan@iit.edu), Ioan Raicu(iraicu@cs.iit.edu)
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
 * This file is part of iit::cs550::finalproj(Diskon: Distributed tasK executiON framework)
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname Xiaobingo,
 *      Hao Chen(hchen71@hawk.iit.edu) with nickname Hchen,
 *		Iman Sadooghi(isadoogh@iit.edu) with nickname Iman,
 *		Ke Wang(kwang22@hawk.iit.edu) with nickname KWang.
 *
 * TSafeMap.h
 *
 *  Created on: Nov 21, 2013
 *      Author: Xiaobingo, Hchen
 *      Contributor: Iman, KWang
 */

#ifndef TSAFEMAP_H_
#define TSAFEMAP_H_

#include <pthread.h>
#include <map>
using namespace std;

/*
 *
 */
template<typename TKey, typename TVal>
class TSafeMap {
	typedef pair<TKey, TVal> PAIR;
	//typedef unordered_map<TKey, TVal> MAP;
	typedef map<TKey, TVal> MAP;
	typedef typename MAP::iterator IT;

public:
	TSafeMap();
	virtual ~TSafeMap();

public:
	void insert(const PAIR &pair);
	void insert(TKey key, TVal val);
	void erase(const TKey &key);

private:
	void init();

private:
	MAP _map;
	pthread_mutex_t _mutex;
};

#endif /* TSAFEMAP_H_ */
