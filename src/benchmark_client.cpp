/*
 * Copyright (C) 2010-2020 DatasysLab@iit.edu(http://datasys.cs.iit.edu/index.html).
 *      Director: Ioan Raicu(iraicu@cs.iit.edu)
 *
 * This file is part of ZHT library(http://datasys.cs.iit.edu/projects/ZHT/index.html).
 *      Ioan Raicu(iraicu@cs.iit.edu),
 *      Tonglin Li(tli13@hawk.iit.edu) with nickname tony,
 *      Xiaobing Zhou(xzhou40@hawk.iit.edu) with nickname xiaobingo.
 *
 * The ZHT Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The ZHT Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the ZHT Library; if not, write to the
 * Data-Intensive Distributed Systems Laboratory, 10 W. 31st Street,
 * Stuart Building, Room 003B, Chicago, IL 60616 USA.
 *
 * zhtclient_test.cpp
 *
 *  Created on: Sep 23, 2012
 *      Author: tony, xiaobingo
 */
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

#include <algorithm>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <netdb.h>
#include <pthread.h>
#include <error.h>

#include "meta.pb.h"
#include "Util.h"

using namespace std;

#include "ZHTClient.h"
#include "ZHTUtil.h"

using namespace iit::datasys::zht::dm;

struct thread_data {
	vector<struct HostEntity> hostList;
	list<string> packageList;
};

vector<struct HostEntity> hostList;
//list<string> myPackagelist;
bool TCP;
int UDP_SOCKET = -1;
int CACHE_SIZE = 1024;

void sig_pipe(int signum) {
	printf("SIGPIPE Caught!\n");
	signal(SIGPIPE, sig_pipe);
}

int benchmarkInsert(string cfgFile, string zht_conf, string neighbor_conf,
		vector<string> &pkgList, ZHTClient &clientRet, int numTest,
		int lenString) {

//	if (clientRet.initialize(cfgFile, neighbor_conf) != 0) { //zhouxb
	if (clientRet.init(zht_conf, neighbor_conf) != 0) {
		cout << "Crap! ZHTClient initialization failed, program exits." << endl;
		return -1;
	}

	for (int i = 0; i < numTest; i++) {
		Package package, package_ret;
		package.set_virtualpath(HashUtil::randomString(lenString)); //as key
		package.set_isdir(true);
		package.set_replicano(5); //orginal--Note: never let it be nagative!!!
		package.set_operation(3); // 3 for insert, 1 for look up, 2 for remove
		package.set_realfullpath(
				"Some-Real-longer-longer-and-longer-Paths--------");
		package.add_listitem("item-----2");
		package.add_listitem("item-----3");
		package.add_listitem("item-----4");
		package.add_listitem("item-----5");
		package.add_listitem("item-----6");
		string str = package.SerializeAsString();
//		cout << "package size = " << str.size() << endl;
//		cout<<"Client.cpp:benchmarkInsert: "<<endl;
//		cout<<"string: "<<str<<endl;
//		cout<<"Insert str: "<<str.c_str()<<endl;
//		cout<<"data(): "<< str.data()<<endl;
		pkgList.push_back(str);
	}
	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;
	vector<string>::iterator it;
	int c = 0;
//	cout << "-----2" << endl;
	for (it = pkgList.begin(); it != pkgList.end(); it++) {
		//	cerr <<"insert count "<< c << endl;
		c++;
		string str_ins = *it;
//cout << "-----1" << endl;
		int ret = clientRet.insert(str_ins);
//cout << "-----2" << endl;
		if (ret < 0) {
			errCount++;
		}
	}
//close(sock);
	end = TimeUtil::getTime_msec();
	cout << "Inserted " << numTest - errCount << " packages out of " << numTest
			<< ", cost " << end - start << " ms" << endl;
	return 0;
}

//pretty much same as benchmarkInsert.
int benchmarkAppend(ZHTClient &client, int numTest, int lenString) {
//	cout << "start to append"<<endl;
//      if (clientRet.initialize(cfgFile, neighbor_conf) != 0) { //zhouxb
	vector<string> pkgList_append;
	for (int i = 0; i < numTest; i++) {
		Package package, package_ret;
		package.set_virtualpath(
				HashUtil::randomString(lenString).append("-append")); //as key
		package.set_isdir(true);
		package.set_replicano(5); //orginal--Note: never let it be nagative!!!
		package.set_operation(4); // 3 for insert, 1 for look up, 2 for remove
		package.set_realfullpath(
				"Some-Real-longer-longer-and-longer-Paths--------");
		package.add_listitem("item-----2");
		package.add_listitem("item-----3");
		package.add_listitem("item-----4");
		package.add_listitem("item-----5");
		package.add_listitem("item-----6");
		string str = package.SerializeAsString();
//              cout << "package size = " << str.size() << endl;
//              cout<<"Client.cpp:benchmarkInsert: "<<endl;
//              cout<<"string: "<<str<<endl;
//              cout<<"Insert str: "<<str.c_str()<<endl;
//              cout<<"data(): "<< str.data()<<endl;
		pkgList_append.push_back(str);
	}
	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;
	vector<string>::iterator it;
	int c = 0;
//cout << "-----0" << endl;
	for (it = pkgList_append.begin(); it != pkgList_append.end(); it++) {
		//      cerr <<"insert count "<< c << endl;
		c++;
		string str_ins = *it;
//cout << "-----1:about to append" << endl;
//	sleep(1);
//		int ret = client.append(str_ins);
		int ret = 0;
		if (ret != 0)
			cout << "client: append ret = " << ret << endl;
//cout << "-----2" << endl;
		if (ret < 0) {
			errCount++;
		}
	}
//close(sock);
	end = TimeUtil::getTime_msec();
	cout << "Appended " << numTest - errCount << " packages out of " << numTest
			<< ", cost " << end - start << " ms" << endl;
	return 0;
}

int benmarkTimeAnalize(string cfgFile, string zht_conf, string neighbor_conf,
		vector<string> &pkgList, ZHTClient &clientRet, int numTest,
		int lenString, string Recordpath) {
	ZHTClient client;

//	if (client.initialize(cfgFile, neighbor_conf) != 0) {
	if (client.init(zht_conf, neighbor_conf) != 0) {
		cout << "Crap! ZHTClient initialization failed, program exits." << endl;
		return -1;
	}

	double timeRecord[numTest]; //={0};
//	bzero(timeRecord, sizeof(timeRecord));
	clientRet = client; //reserve this client object for other benchmark(lookup/remove) to use.

	//vector<string> pkgList;
	int i = 0;
	for (i = 0; i < numTest; i++) {
		Package package, package_ret;
		package.set_virtualpath(HashUtil::randomString(lenString)); //as key
		package.set_isdir(true);
		package.set_replicano(5); //orginal--Note: never let it be nagative!!!
		package.set_operation(3); // 3 for insert, 1 for look up, 2 for remove
		package.set_realfullpath(
				"Some-Real-longer-longer-and-longer-Paths--------");
		package.add_listitem("item-----1");
		package.add_listitem("item-----2");
		package.add_listitem("item-----3");
		package.add_listitem("item-----4");
		package.add_listitem("item-----5");
		string str = package.SerializeAsString();
//		cout << "package size = " << str.size() << endl;
//		cout<<"Client.cpp:benchmarkInsert: "<<endl;
//		cout<<"string: "<<str<<endl;
//		cout<<"Insert str: "<<str.c_str()<<endl;
//		cout<<"data(): "<< str.data()<<endl;

		pkgList.push_back(str);
	}

	double start = 0;
	double end = 0;
	double istart = 0;
	double iend = 0;
	int errCount = 0;
	vector<string>::iterator it;
	int c = 0;

	ofstream record;
	record.open(Recordpath.c_str());

	start = TimeUtil::getTime_msec();
	for (it = pkgList.begin(); it != pkgList.end(); it++) {
//		cout<<c<<endl;
		c++;
		double interval = 0;
		istart = TimeUtil::getTime_usec();
		int op_ret = client.insert((*it));
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

	cout << "Inserted " << numTest - errCount << " packages out of " << numTest
			<< ", cost " << end - start << " ms" << endl;

	return 0;
}

float benchmarkLookup(vector<string> strList, ZHTClient &client) {
	vector<string>::iterator it;

	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;
//cout << "Client: benchmarkLookup: start lookup \n";
	int c = 0;
	for (it = strList.begin(); it != strList.end(); it++) {
		string result;
//		cout << c << endl;
		c++;
//		cout<<"lookup: "<<c<<endl;
//sleep(1);
//
//		cout << "Client: What I want to find: \n";
//		cout <<"Lookup: "<< (*it).c_str() << endl;
		if (client.lookup((*it), result) < 0) {

			errCount++;
		} else if (result.empty()) { //empty string
			errCount++;
		}
//		cout << "Client: What I get: ";
//		cout << result.c_str() << endl;
	}

	end = TimeUtil::getTime_msec();

	cout << "Lookup " << strList.size() - errCount << " packages out of "
			<< strList.size() << ", cost " << end - start << " ms" << endl;
	return 0;
}

float benchmarkRemove(vector<string> strList, ZHTClient &client) {
	vector<string>::iterator it;

	for (it = strList.begin(); it != strList.end(); it++) {
		Package package;
		package.ParseFromString((*it));
		package.set_operation(2); // 3 for insert, 1 for look up, 2 for remove
		package.set_replicano(5); //5: original, 3 not original

		strList.erase(it);
		string newStr = package.SerializeAsString();
		strList.push_back(newStr);
	}

	double start = 0;
	double end = 0;
	start = TimeUtil::getTime_msec();
	int errCount = 0;
	int c = 0;
	for (it = strList.begin(); it != strList.end(); it++) {
		string result;
		c++;
//		cout <<"Remove count "<< c << endl;

//		cout <<"Remove: "<< (*it).c_str() << endl;
		if (client.remove((*it)) < 0) {
			errCount++;
		}

	}

	end = TimeUtil::getTime_msec();

	cout << "Remove " << strList.size() - errCount << " packages out of "
			<< strList.size() << ", cost " << end - start << " ms" << endl;
	return 0;
}
//This is an example.

int benchmarkALL(int numTest, int strLen) { //103+length
//	int para = strLen - 128;
	return 0;
}

int main(int argc, char* argv[]) {

//	cout << "Usage: ./client <num_operations> <neighbor_conf> <configFile>"<< endl;
	/*	//	For BGP
	 const string cmd = "cat /proc/personality.sh | grep BG_PSETORG";
	 string torusID = executeShell(cmd);
	 torusID.resize(torusID.size() - 1);
	 int pid = getpid();
	 unsigned int v = myhash(torusID.c_str(), 1000000) + pid ;
	 //cout<<"client: pid = "<<pid<<endl;

	 //cout<<"Client random n = "<<v<<endl;
	 srand(v);
	 */
	srand(getpid() + TimeUtil::getTime_usec());
	char* isTCP = argv[4];
//	cout<<"Protocol = "<<isTCP<<endl;
	/*if (!strcmp("TCP", isTCP)) {
	 TCP = true;
	 } else {
	 TCP = false;
	 }*/

	TCP = true;

	string zht_conf; //todo:

	int numOper = atoi(argv[1]);
//cout<<"numOper = "<<numOper<<endl;
	string cfgFile(argv[3]);
//cout<<"cfgFile = "<<cfgFile<<endl;
	string neighbor_conf(argv[2]);
//cout<<"neighbor_conf: "<<neighbor_conf<<endl;
	vector<string> pkgList;
	ZHTClient testClient;
//	int pid = getpid();
	char* tmpStr;
	stringstream ss; //create a stringstream
//	ss << pid;

//	string recordFile = "record." + ss.str();
//	benmarkTimeAnalize(cfgFile, neighbor_conf, pkgList, testClient, numOper, 15, recordFile);
//cout<<"start to insert..."<<endl;

	int lk = 0;
	int rm = 0;
	int in = 0;

	int c;
	while ((c = getopt(argc, argv, "ilr")) != -1) {
		switch (c) {
		case 'l':
			lk = 1;
			break;
		case 'r':
			rm = 1;
			break;
		case 'i':
			in = 1;
			break;
		default:
			fprintf(stderr, "Illegal argument \"%c\"\n", c);
			exit(1);
		}
	}

	benchmarkInsert(cfgFile, zht_conf, neighbor_conf, pkgList, testClient,
			numOper, 15); //25fro 128bytes.

	//cout << "Client:main, start lookup \n";
	benchmarkLookup(pkgList, testClient);
	//sleep(2);
//	benchmarkAppend(testClient, numOper, 15);

	benchmarkRemove(pkgList, testClient);
//	testClient.tearDownTCP(TCP);
//	testClient.tearDownTCP(); //zhouxb
	return 0;

}
