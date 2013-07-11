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
 * benchmark_client.cpp
 *
 *  Created on: Sep 23, 2012
 *      Author: Tony
 *      Contributor: Xiaobingo
 */
#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include <error.h>

#include "meta.pb.h"
#include "Util.h"

#include "ZHTClient.h"
#include "ZHTUtil.h"

using namespace std;
using namespace iit::datasys::zht::dm;

int benchmarkInsert(vector<string> &pkgList, ZHTClient &zc, int numOfOps,
		int lenString) {

	for (int i = 0; i < numOfOps; i++) {

		Package package, package_ret;
		package.set_virtualpath(HashUtil::randomString(lenString)); //as key
		package.set_isdir(true);
		package.set_replicano(5); //orginal--Note: never let it be nagative!!!
		package.set_realfullpath(
				"Some-Real-longer-longer-and-longer-Paths--------");
		package.add_listitem("item-----2");
		package.add_listitem("item-----3");
		package.add_listitem("item-----4");
		package.add_listitem("item-----5");
		package.add_listitem("item-----6");
		string str = package.SerializeAsString();
		pkgList.push_back(str);
	}

	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;

	int c = 0;
	vector<string>::iterator it;
	for (it = pkgList.begin(); it != pkgList.end(); it++) {

		c++;
		string str_ins = *it;

		int ret = zc.insert(str_ins);

		if (ret < 0) {
			errCount++;
		}
	}

	end = TimeUtil::getTime_msec();
	cout << "Inserted " << numOfOps - errCount << " packages out of "
			<< numOfOps << ", cost " << end - start << " ms" << endl;
	return 0;
}

int benchmarkAppend(ZHTClient &zc, int numOfOps, int lenString) {

	vector<string> pkgList_append;

	for (int i = 0; i < numOfOps; i++) {

		Package package, package_ret;
		package.set_virtualpath(
				HashUtil::randomString(lenString).append("-append")); //as key
		package.set_isdir(true);
		package.set_replicano(5); //orginal--Note: never let it be nagative!!!
		package.set_realfullpath(
				"Some-Real-longer-longer-and-longer-Paths--------");
		package.add_listitem("item-----2");
		package.add_listitem("item-----3");
		package.add_listitem("item-----4");
		package.add_listitem("item-----5");
		package.add_listitem("item-----6");
		string str = package.SerializeAsString();
		pkgList_append.push_back(str);
	}

	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;

	int c = 0;
	vector<string>::iterator it;
	for (it = pkgList_append.begin(); it != pkgList_append.end(); it++) {

		c++;
		string str_ins = *it;
		int ret = zc.append(str_ins);

		if (ret < 0) {
			errCount++;
		}
	}

	end = TimeUtil::getTime_msec();
	cout << "Appended " << numOfOps - errCount << " packages out of "
			<< numOfOps << ", cost " << end - start << " ms" << endl;
	return 0;
}

int benmarkTimeAnalize(vector<string> &pkgList, ZHTClient &zc, int numOfOps,
		int lenString, string Recordpath) {

	double timeRecord[numOfOps]; //={0};

	int i = 0;
	for (i = 0; i < numOfOps; i++) {

		Package package, package_ret;
		package.set_virtualpath(HashUtil::randomString(lenString)); //as key
		package.set_isdir(true);
		package.set_replicano(5); //orginal--Note: never let it be nagative!!!
		package.set_realfullpath(
				"Some-Real-longer-longer-and-longer-Paths--------");
		package.add_listitem("item-----1");
		package.add_listitem("item-----2");
		package.add_listitem("item-----3");
		package.add_listitem("item-----4");
		package.add_listitem("item-----5");
		string str = package.SerializeAsString();

		pkgList.push_back(str);
	}

	double start = 0;
	double end = 0;
	double istart = 0;
	double iend = 0;
	int errCount = 0;

	ofstream record;
	record.open(Recordpath.c_str());

	start = TimeUtil::getTime_msec();

	int c = 0;
	vector<string>::iterator it;
	for (it = pkgList.begin(); it != pkgList.end(); it++) {

		c++;
		double interval = 0;
		istart = TimeUtil::getTime_usec();
		int op_ret = zc.insert((*it));
		iend = TimeUtil::getTime_usec();

		if (op_ret < 0) {
			errCount++;
			interval = -1;
		} else
			interval = iend - istart;
		record << interval << endl;
		timeRecord[c] = interval;

	}

	end = TimeUtil::getTime_msec();
	record.close();

	cout << "Inserted " << numOfOps - errCount << " packages out of "
			<< numOfOps << ", cost " << end - start << " ms" << endl;

	return 0;
}

float benchmarkLookup(vector<string> strList, ZHTClient &zc) {

	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;

	int c = 0;
	vector<string>::iterator it;
	for (it = strList.begin(); it != strList.end(); it++) {

		string result;
		c++;

		if (zc.lookup((*it), result) < 0) {

			errCount++;
		} else if (result.empty()) { //empty string
			errCount++;
		}
	}

	end = TimeUtil::getTime_msec();

	cout << "Lookup " << strList.size() - errCount << " packages out of "
			<< strList.size() << ", cost " << end - start << " ms" << endl;
	return 0;
}

float benchmarkRemove(vector<string> strList, ZHTClient &zc) {

	vector<string> removes;

	vector<string>::iterator it;
	for (it = strList.begin(); it != strList.end(); it++) {

		Package package;
		package.ParseFromString((*it));
		package.set_replicano(5); //5: original, 3 not original

		string newStr = package.SerializeAsString();
		removes.push_back(newStr);
	}

	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;

	int c = 0;
	for (it = removes.begin(); it != removes.end(); it++) {

		string result;
		c++;

		if (zc.remove((*it)) < 0) {
			errCount++;
		}
	}

	end = TimeUtil::getTime_msec();

	cout << "Remove " << strList.size() - errCount << " packages out of "
			<< strList.size() << ", cost " << end - start << " ms" << endl;
	return 0;
}

int benchmark(string &zhtConf, string &neighborConf, int numOfOps) {

	srand(getpid() + TimeUtil::getTime_usec());

	vector<string> pkgList;
	ZHTClient zc;

	if (zc.init(zhtConf, neighborConf) != 0) {

		cout << "Crap! ZHTClient initialization failed, program exits." << endl;
		return -1;
	}

	benchmarkInsert(pkgList, zc, numOfOps, 15);

	benchmarkLookup(pkgList, zc);

	benchmarkAppend(zc, numOfOps, 15);

	benchmarkRemove(pkgList, zc);

	zc.teardown();

	return 0;

}

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	int printHelp = 0;
	int numOfOps = -1;
	string zhtConf = "";
	string neighborConf = "";

	int c;
	while ((c = getopt(argc, argv, "z:n:o:h")) != -1) {
		switch (c) {
		case 'z':
			zhtConf = string(optarg);
			break;
		case 'n':
			neighborConf = string(optarg);
			break;
		case 'o':
			numOfOps = atoi(optarg);
			break;
		case 'h':
			printHelp = 1;
			break;
		default:
			fprintf(stderr, "Illegal argument \"%c\"\n", c);
			printUsage(argv[0]);
			exit(1);
		}
	}

	int helpPrinted = 0;
	if (printHelp) {
		printUsage(argv[0]);
		helpPrinted = 1;
	}

	try {
		if (!zhtConf.empty() && !neighborConf.empty() && numOfOps != -1) {

			benchmark(zhtConf, neighborConf, numOfOps);

		} else {

			if (!helpPrinted)
				printUsage(argv[0]);
		}
	} catch (exception& e) {

		fprintf(stderr, "%s, exception caught:\n\t%s",
				"benchmark_client.cpp::main", e.what());
	}

}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"-z zht.conf -n neighbor.conf -o number_of_operations [-h(help)]");
}

