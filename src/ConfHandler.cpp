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
 * ConfHandler.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: tony, xiaobingo
 */

#include "ConfHandler.h"
#include "ConfEntry.h"
#include "StrTokenizer.h"

#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
using namespace std;

namespace iit {
namespace datasys {
namespace zht {
namespace dm {

ConfHandler::VEC ConfHandler::NeighborVector = VEC();
ConfHandler::MAP ConfHandler::NeighborSeeds = MAP();
ConfHandler::MAP ConfHandler::ZHTParameters = MAP();
ConfHandler::MAP ConfHandler::NodeParameters = MAP();

string ConfHandler::CONF_ZHT = "zht.conf";
string ConfHandler::CONF_NODE = "node.conf";
string ConfHandler::CONF_NEIGHBOR = "neighbor.conf";

uint ConfHandler::ZC_MAX_ZHT = 0;
uint ConfHandler::ZC_NUM_REPLICAS = 0;
uint ConfHandler::ZC_REPLICATION_TYPE = 0;

uint ConfHandler::NC_ZHT_CAPACITY = 0;
string ConfHandler::NC_FILECLIENT_PATH = "./fileclient";
string ConfHandler::NC_FILESERVER_PATH = "./file_server.exe";
uint ConfHandler::NC_FILESERVER_PORT = 9000;

string ConfHandler::ZC_HTDATA_PATH = ""; //todo: empty string not allowed.
uint ConfHandler::ZC_MIGSLP_TIME = 1000000; //micro second

ConfHandler::ConfHandler() {

}

ConfHandler::~ConfHandler() {

}

string ConfHandler::getProtocolFromConf() {

	ConfHandler::MAP *zpmap = &ConfHandler::ZHTParameters;

	ConfHandler::MIT it;

	for (it = zpmap->begin(); it != zpmap->end(); it++) {

		ConfEntry ce;
		ce.assign(it->first);

		if (ce.name() == Const::PROTO_NAME) {

			return ce.value();
		}
	}

	return "";
}

string ConfHandler::getPortFromConf() {

	ConfHandler::MAP *zpmap = &ConfHandler::ZHTParameters;

	ConfHandler::MIT it;

	for (it = zpmap->begin(); it != zpmap->end(); it++) {

		ConfEntry ce;
		ce.assign(it->first);

		if (ce.name() == Const::PROTO_PORT) {

			return ce.value();
		}
	}

	return "";
}

void ConfHandler::initConf(string zhtConf, string neighborConf) {

	ConfHandler::CONF_ZHT = zhtConf; //zht.conf
	ConfHandler::CONF_NEIGHBOR = neighborConf; //neighbor.conf

	ConfHandler::setZHTParameters(zhtConf);
	ConfHandler::setNeighborSeeds(neighborConf);
}

void ConfHandler::setNeighborSeeds(const string& neighborCfg) {

	setParametersInternal(neighborCfg, NeighborSeeds);

	setNeighborVector(NeighborVector);
}

void ConfHandler::setZHTParameters(const string& zhtConfig) {

	setParametersInternal(zhtConfig, ZHTParameters);

	pickZHTParameters();
}

void ConfHandler::setNodeParameters(const string& nodeConfig) {

	setParametersInternal(nodeConfig, NodeParameters);

//	pickNodeParameters();
}

void ConfHandler::setParametersInternal(string configFile, MAP& configMap) {

	ifstream ifs(configFile.c_str(), ifstream::in);

	const char *delimiter = Const::CONF_DELIMITERS.c_str();

	string line;
	while (getline(ifs, line)) {

		string remains = line;

		if (remains.empty())
			continue;

		if (remains.substr(0, 1) == "#") //starts with #, means comment
			continue;

		StrTokenizer strtok(remains);

		string one;
		string two;

		if (strtok.has_more_tokens())
			one = strtok.next_token();

		if (strtok.has_more_tokens())
			two = strtok.next_token();

		if (one.empty() || two.empty())
			continue;

		string name = one;
		string value = two;

		if (!name.empty() && !value.empty()) {

			ConfEntry ce(name, value);
			configMap.insert(PAIR(ce.toString(), ce)); //todo: use hash code to reduce size of key/value pair.
		}
	}

	ifs.close();
}

void ConfHandler::setNeighborVector(VEC &neighborVector) {

	ConfHandler::MIT kvi;
	ConfHandler::MAP* map = &ConfHandler::NeighborSeeds;

	for (kvi = map->begin(); kvi != map->end(); kvi++) {

		neighborVector.push_back(kvi->second);
	}
}

void ConfHandler::pickNodeParameters() {

	ConfHandler::MIT kvi;
	ConfHandler::MAP* map = &ConfHandler::NodeParameters;

	for (kvi = map->begin(); kvi != map->end(); kvi++) {

		ConfEntry kv = kvi->second;

		if (kv.name() == Const::NC_ZHT_CAPACITY) {

			NC_ZHT_CAPACITY = atoi(kv.value().c_str());
		} else if (kv.name() == Const::NC_FILECLIENT_PATH) {

			NC_FILECLIENT_PATH = kv.value();
		} else if (kv.name() == Const::NC_FILESERVER_PATH) {

			NC_FILESERVER_PATH = kv.value();
		} else if (kv.name() == Const::NC_FILESERVER_PORT) {

			NC_FILESERVER_PORT = atoi(kv.value().c_str());
		} else {
		}
	}
}

void ConfHandler::pickZHTParameters() {

	ConfHandler::MIT kvi;
	ConfHandler::MAP* map = &ConfHandler::ZHTParameters;

	for (kvi = map->begin(); kvi != map->end(); kvi++) {

		ConfEntry kv = kvi->second;

		if (kv.name() == Const::ZC_MAX_ZHT) {

			ZC_MAX_ZHT = atoi(kv.value().c_str());
		} else if (kv.name() == Const::ZC_NUM_REPLICAS) {

			ZC_NUM_REPLICAS = atoi(kv.value().c_str());
		} else if (kv.name() == Const::ZC_REPLICATION_TYPE) {

			ZC_REPLICATION_TYPE = atoi(kv.value().c_str());
		} else if (kv.name() == Const::NC_ZHT_CAPACITY) {

			NC_ZHT_CAPACITY = atoi(kv.value().c_str());
		} else if (kv.name() == Const::NC_FILECLIENT_PATH) {

			NC_FILECLIENT_PATH = kv.value();
		} else if (kv.name() == Const::NC_FILESERVER_PATH) {

			NC_FILESERVER_PATH = kv.value();
		} else if (kv.name() == Const::NC_FILESERVER_PORT) {

			NC_FILESERVER_PORT = atoi(kv.value().c_str());
		} else if (kv.name() == Const::ZC_HTDATA_PATH) {

			ZC_HTDATA_PATH = kv.value();
		} else if (kv.name() == Const::ZC_MIGSLP_TIME) {

			ZC_MIGSLP_TIME = atoi(kv.value().c_str());
		} else {

		}
	}
}

} /* namespace dm */
} /* namespace zht */
} /* namespace datasys */
} /* namespace iit */
