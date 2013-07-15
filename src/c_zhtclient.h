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
 * c_zhtclient.h
 *
 *  Created on: Aug 7, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#ifndef C_ZHTCLIENT_H_
#define C_ZHTCLIENT_H_

#ifdef __cplusplus
# define ZHT_CPP(x) x
#else
# define ZHT_CPP(x)
#endif

#include <stddef.h>

ZHT_CPP(extern "C" {)

	/* wrapp C++ ZHTClient::initialize.
	 * return code: 0 if succeeded, or -1 if failed.
	 * */
	int c_zht_init(const char *zhtConfig, const char *neighborConf);

	/* wrapp C++ ZHTClient::lookup.
	 * KEY: empty key not allowed, if empty, return -1, means failed.
	 * RESULT: lookup result
	 * return code: 0 if succeeded, or -1 if empty key, or , -98 if unrecognized operation.
	 * */
	int c_zht_lookup(const char *key, char *result);

	/* wrapp C++ ZHTClient::remove.
	 * KEY: empty key not allowed, if empty, return -1, means failed.
	 * return code: 0 if succeeded, or -1 if empty key, or , -98 if unrecognized operation.
	 * */
	int c_zht_remove(const char *key);

	/* wrapp C++ ZHTClient::insert.
	 * KEY: empty key not allowed, if empty, return -1, means failed.
	 * VALUE: empty value ignored.
	 * return code: 0 if succeeded, or -1 if empty key, or , -98 if unrecognized operation.
	 * */
	int c_zht_insert(const char *key, const char *value);

	/* wrapp C++ ZHTClient::append.
	 * PAIR is expected to be a serialization string with protocol-buffer-c-binding representation.
	 * return code: 0 if succeeded, or -1 if empty key, or -2 if failed, -98 if unrecognized operation.
	 * */

	int c_zht_append(const char *key, const char *value);

	/* wrapp C++ ZHTClient::teardown.
	 * return code: 0 if succeeded, or -1 if failed.
	 * */
	int c_zht_teardown();

	int c_zht_compare_swap(const char *key, const char *seen_value,
			const char *new_value, char **value_querie);

	ZHT_CPP (})

#endif
