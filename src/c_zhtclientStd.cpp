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
 * c_zhtclientStd.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: Corentin Debains
 *      Contributor: Tony, Xiaobingo, KWang, DZhao
 */

#include "c_zhtclientStd.h"

#include "cpp_zhtclient.h"
#include "meta.pb.h"

#include <string.h>
#include <stdio.h>

using namespace std;

int c_zht_init_std(ZHTClient_c *zhtClient, const char *zhtConfig,
		const char *neighborConf) {

	ZHTClient *zhtcppClient = new ZHTClient();

	string zhtConfigStr(zhtConfig);
	string neighborConfStr(neighborConf);

	if (zhtcppClient->init(zhtConfigStr, neighborConfStr) != 0) {

		printf("ZHTClient initialization failed, program exits.");
		return -1;
	}

	*zhtClient = (ZHTClient_c) zhtcppClient;

	return 0;
}

int c_zht_lookup_std(ZHTClient_c zhtClient, const char *key, char *result) {

	ZHTClient *zhtcppClient = (ZHTClient *) zhtClient;

	string skey(key);

	string resultStr;
	int ret = zhtcppClient->lookup(skey, resultStr);

	strncpy(result, resultStr.c_str(), resultStr.size());

	return ret;
}

int c_zht_remove_std(ZHTClient_c zhtClient, const char *key) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string skey(key);

	return zhtcppClient->remove(skey);
}

int c_zht_insert_std(ZHTClient_c zhtClient, const char *key,
		const char *value) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string skey(key);
	string sval(value);

	return zhtcppClient->insert(skey, sval);
}

int c_zht_append_std(ZHTClient_c zhtClient, const char *key,
		const char *value) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string skey(key);
	string sval(value);

	return zhtcppClient->append(skey, sval);

}

int c_zht_compare_swap_std(ZHTClient_c zhtClient, const char *key,
		const char *seen_value, const char *new_value, char *value_queried) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string skey(key);
	string sseenValue(seen_value);
	string snewValue(new_value);

	string resultStr;
	int rc = zhtcppClient->compare_swap(skey, sseenValue, snewValue, resultStr);

	strncpy(value_queried, resultStr.c_str(), resultStr.size());

	return rc;
}

int c_state_change_callback_std(ZHTClient_c zhtClient, const char *key,
		const char *expeded_val) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	string skey(key);
	string expededval(expeded_val);

	int rc = zhtcppClient->state_change_callback(skey, expededval);

	return rc;
}

int c_zht_teardown_std(ZHTClient_c zhtClient) {

	ZHTClient * zhtcppClient = (ZHTClient *) zhtClient;

	int rc = zhtcppClient->teardown();

	delete zhtcppClient;
	zhtcppClient = NULL;

	return rc;
}

