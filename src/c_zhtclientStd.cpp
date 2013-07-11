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
 * c_zhtclientStd.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: Corentin Debains
 *      Contributor: Tony, Xiaobingo
 */

#include "c_zhtclientStd.h"

#include "ZHTClient.h"
#include "meta.pb.h"

#include <string.h>
#include <stdio.h>

using namespace std;

int c_zht_init_std(ZHTClient_c * zhtClient, const char *zhtConfig,
		const char *neighborConf) {

	ZHTClient * zhtcppClient = new ZHTClient();

	string zhtStr(zhtConfig);
	string memberStr(neighborConf);

	if (zhtcppClient->init(zhtStr, memberStr) != 0) {
		printf("Crap! ZHTClient initialization failed, program exits.");

		return -1;
	}

	*zhtClient = (ZHTClient_c) zhtcppClient;

	return 0;
}

int c_zht_lookup_std(ZHTClient_c zhtClient, const char *pair, char *result,
		size_t *n) {

	ZHTClient *zhtcppClient = (ZHTClient *) zhtClient;

	string sPair(pair);

	string resultStr;
	int ret = zhtcppClient->lookup(sPair, resultStr);

	/*
	 * hello,zht:hello,zht ==> hello,zht:zht
	 * */
	string store;
	char * pch, *sp;
	pch = strtok_r((char*) resultStr.c_str(), ":", &sp);
	Package package2;

	while (pch != NULL) {

		package2.ParseFromString(pch);
		string strRealfullpath = package2.realfullpath();

		store.append(strRealfullpath);
		store.append(":");

		pch = strtok_r(NULL, ":", &sp);
	}

	size_t found = store.find_last_of(":");
	store = store.substr(0, found);
	package2.set_realfullpath(store);

	store = package2.SerializeAsString();
	strncpy(result, store.c_str(), strlen(store.c_str()));
	*n = store.size();

	return ret;
}

int c_zht_lookup2_std(ZHTClient_c zhtClient, const char *key, char *result,
		size_t *n) {

	ZHTClient *zhtcppClient = (ZHTClient *) zhtClient;

	string sKey(key);

	Package package;
	package.set_virtualpath(sKey); //as key
	package.set_isdir(true);
	package.set_replicano(5);
	package.set_operation(1); //1 for look up, 2 for remove, 3 for insert, 4 append

	string resultStr;
	int ret = zhtcppClient->lookup(package.SerializeAsString(), resultStr);

	/*
	 * hello,zht:hello,zht ==> hello,zht:zht
	 * */
	string store;
	char * pch, *sp;
	pch = strtok_r((char*) resultStr.c_str(), ":", &sp);
	Package package2;

	while (pch != NULL) {

		package2.ParseFromString(pch);
		string strRealfullpath = package2.realfullpath();

		store.append(strRealfullpath);
		store.append(":");

		pch = strtok_r(NULL, ":", &sp);
	}

	size_t found = store.find_last_of(":");
	store = store.substr(0, found);

	strncpy(result, store.c_str(), strlen(store.c_str()));
	*n = store.size();

	return ret;
}

int c_zht_remove_std(ZHTClient_c zhtClient, const char *pair) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string str(pair);

	return zhtcppClient->remove(str);
}

int c_zht_remove2_std(ZHTClient_c zhtClient, const char *key) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string sKey(key);

	Package package;
	package.set_virtualpath(sKey);
	package.set_operation(2); //1 for look up, 2 for remove, 3 for insert, 4 append

	return zhtcppClient->remove(package.SerializeAsString());
}

int c_zht_insert_std(ZHTClient_c zhtClient, const char *pair) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string str(pair);

	return zhtcppClient->insert(str);
}

int c_zht_insert2_std(ZHTClient_c zhtClient, const char *key,
		const char *value) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string sKey(key);
	string sValue(value);

	Package package;
	package.set_virtualpath(sKey); //as key
	package.set_isdir(true);
	package.set_replicano(5);
	package.set_operation(3); //1 for look up, 2 for remove, 3 for insert, 4 append
	if (!sValue.empty())
		package.set_realfullpath(sValue);

	return zhtcppClient->insert(package.SerializeAsString());
}

int c_zht_append_std(ZHTClient_c zhtClient, const char *pair) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string str(pair);

	return zhtcppClient->append(str);
}

int c_zht_append2_std(ZHTClient_c zhtClient, const char *key,
		const char *value) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string sKey(key);
	string sValue(value);

	Package package;
	package.set_virtualpath(sKey); //as key
	package.set_isdir(true);
	package.set_replicano(5);
	package.set_operation(4); //1 for look up, 2 for remove, 3 for insert, 4 append
	if (!sValue.empty())
		package.set_realfullpath(sValue);

	return zhtcppClient->append(package.SerializeAsString());

}

int c_zht_teardown_std(ZHTClient_c zhtClient) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	return zhtcppClient->teardown();
}

int c_zht_compare_and_swap_std(ZHTClient_c zhtClient, const char *key,
		const char *seen_value, const char *new_value, char **value_queried) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;
	string sKey(key);
	string sSeenValue(seen_value);
	string sNewValue(new_value);

	Package package;
	package.set_virtualpath(sKey);
	package.set_isdir(true);
	package.set_replicano(5);
	package.set_operation(5);  // 5 for comapre and swap
	if (!sSeenValue.empty()) {
		package.set_realfullpath(sSeenValue);
	}
	if (!sNewValue.empty()) {
		package.set_newfullpath(sNewValue);
	}

	int rc = 0;

	string return_str;
	/*rc = zhtcppClient->compare_and_swap(package.SerializeAsString(),
	 return_str);*/

	Package ret_pack;
	ret_pack.ParseFromString(return_str);

	strcpy(*value_queried, ret_pack.realfullpath().c_str());

	return rc;
}
