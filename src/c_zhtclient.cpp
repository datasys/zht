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
 * c_zhtclient.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony, KWang, DZhao
 */

#include "c_zhtclient.h"
#include "c_zhtclientStd.h"
#include "lock_guard.h"
#include <pthread.h>

ZHTClient_c zhtClient;

pthread_mutex_t c_zht_client_mutex;

int c_zht_init(const char *zhtConfig, const char *neighborConf) {

	pthread_mutex_init(&c_zht_client_mutex, NULL);

	return c_zht_init_std(&zhtClient, zhtConfig, neighborConf);
}

int c_zht_lookup(const char *pair, char *result, size_t *n) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_lookup_std(zhtClient, pair, result, n);
}

int c_zht_lookup2(const char *key, char *result, size_t *n) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_lookup2_std(zhtClient, key, result, n);
}

int c_zht_remove(const char *pair) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_remove_std(zhtClient, pair);
}

int c_zht_remove2(const char *key) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_remove2_std(zhtClient, key);
}

int c_zht_insert(const char *pair) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_insert_std(zhtClient, pair);
}

int c_zht_insert2(const char *key, const char *value) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_insert2_std(zhtClient, key, value);
}

int c_zht_append(const char *pair) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_append_std(zhtClient, pair);
}

int c_zht_append2(const char *key, const char *value) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_append2_std(zhtClient, key, value);
}

int c_zht_teardown() {

	pthread_mutex_destroy(&c_zht_client_mutex);

	return c_zht_teardown_std(zhtClient);
}

int c_zht_compare_and_swap(const char *key, const
char *seen_value, const char *new_value, char **value_queried) {

	lock_guard lock(&c_zht_client_mutex);

	return c_zht_compare_and_swap_std(zhtClient, key, seen_value, new_value,
			value_queried);
}

