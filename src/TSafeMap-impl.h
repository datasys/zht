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
 * TSafeMap-impl.h
 *
 *  Created on: Nov 21, 2013
 *      Author: Xiaobingo, Hchen
 *      Contributor: Iman, KWang
 */

#ifndef TSAFEMAP_IMPL_H_
#define TSAFEMAP_IMPL_H_

#include "TSafeMap.h"
#include "lock_guard.h"

template<typename TKey, typename TVal>
TSafeMap<TKey, TVal>::TSafeMap() {
	init();
}

template<typename TKey, typename TVal>
TSafeMap<TKey, TVal>::~TSafeMap() {

	pthread_mutex_destroy (&_mutex);
}

template<typename TKey, typename TVal>
void TSafeMap<TKey, TVal>::init() {

	pthread_mutex_init(&_mutex, NULL);
}

template<typename TKey, typename TVal>
void TSafeMap<TKey, TVal>::insert(const PAIR &pair) {

	lock_guard lock(&_mutex);
	_map.insert(pair);
}

template<typename TKey, typename TVal>
void TSafeMap<TKey, TVal>::insert(TKey key, TVal val) {

	lock_guard lock(&_mutex);
	_map.insert(make_pair(key, val));
}

template<typename TKey, typename TVal>
void TSafeMap<TKey, TVal>::erase(const TKey &key) {

	lock_guard lock(&_mutex);

	IT it = _map.find(key);

	if (it != _map.end())
		_map.erase(it);
}

#endif /* TSAFEMAP_IMPL_H_ */
