/*
 * ZHTServer.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: Xiaobing Zhou
 */

#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
using namespace std;

#include "EpollServer.h"
#include "ConfHandler.h"
using namespace iit::datasys::zht::dm;

//#include "mpi_server.h"
#include "ip_server.h"

void printUsage(char *argv_0);

string getProtocolFromConf() {

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

string getPortFromConf() {

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

void init_conf(string zhtConf, string neighborConf) {

	ConfHandler::CONF_ZHT = zhtConf; //zht.conf
	ConfHandler::CONF_NEIGHBOR = neighborConf; //neighbor.conf

	ConfHandler::setZHTParameters(zhtConf);
	ConfHandler::setNeighborSeeds(neighborConf);
}

int main(int argc, char **argv) {

	extern char *optarg;

	int printHelp = 0;
	string protocol = Const::StringEmpty;
	string port = Const::StringEmpty;
	string zhtConf = Const::StringEmpty;
	string neighborConf = Const::StringEmpty;

	int c;
	while ((c = getopt(argc, argv, "z:n:h")) != -1) {
		switch (c) {
		case 'z':
			zhtConf = string(optarg);
			break;
		case 'n':
			neighborConf = string(optarg);
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
		if (!zhtConf.empty() && !neighborConf.empty()) {

			/*init config*/
			init_conf(zhtConf, neighborConf);

			/*get protocol and port*/
			protocol = getProtocolFromConf();
			port = getPortFromConf();

			if (protocol.empty()) {

				cout << "zht.conf: protocol not configured" << endl;
				exit(1);
			}

			if (port.empty()) {

				cout << "zht.conf: port not configured" << endl;
				exit(1);
			}

			char buf[100];
			memset(buf, 0, sizeof(buf));
			int n = sprintf(buf, "ZHT server- <localhost:%s> started...\n",
					port.c_str());

			cout << buf << endl;

			IPServer *is = new IPServer();
			EpollServer es(is);
			es.serve(port.c_str());

		} else {

			if (!helpPrinted)
				printUsage(argv[0]);
		}
	} catch (exception& e) {

		fprintf(stderr, "%s, exception caught:\n\t%s", "ZHTServer::main",
				e.what());
	}

}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"-z zht.conf -n neighbor.conf [-h(help)]");
}
