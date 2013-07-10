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
 * ZHTServer.cpp
 *
 *  Created on: Jun 26, 2013
 *      Author: Xiaobingo
 *      Contributor: Tony
 */

#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
using namespace std;

#ifdef PF_INET
#include "EpollServer.h"
#include "ip_server.h"
#elif MPI_INET
#include "mpi_server.h"
#endif

#include "ConfHandler.h"
using namespace iit::datasys::zht::dm;

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	int printHelp = 0;
	string protocol = Const::StringEmpty;
	string port_from_input = Const::StringEmpty;
	string port_from_conf = Const::StringEmpty;
	string zhtConf = Const::StringEmpty;
	string neighborConf = Const::StringEmpty;

	int c;
	while ((c = getopt(argc, argv, "z:n:p:h")) != -1) {
		switch (c) {
		case 'z':
			zhtConf = string(optarg);
			break;
		case 'n':
			neighborConf = string(optarg);
			break;
		case 'p':
			port_from_input = string(optarg);
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
			ConfHandler::initConf(zhtConf, neighborConf);

			/*get protocol and port*/
			protocol = ConfHandler::getProtocolFromConf();

			port_from_conf = ConfHandler::getPortFromConf();
			if (port_from_conf.empty()) {

				cout << "zht.conf: port not configured" << endl;
			}

			string port =
					!port_from_input.empty() ? port_from_input : port_from_conf;

			if (port.empty()) {

				cout << "zht server: port not defined by user" << endl;
				exit(1);
			}

			if (protocol.empty()) {

				cout << "zht.conf: protocol not configured" << endl;
				exit(1);
			}

			char buf[100];
			memset(buf, 0, sizeof(buf));
			int n = sprintf(buf,
					"ZHT server- <localhost:%s> <protocol:%s> started...",
					port.c_str(), protocol.c_str());

			cout << buf << endl;

#ifdef PF_INET

			EpollServer es(port.c_str(), new IPServer());
			es.serve();
#elif MPI_INET

			MPIServer mpis(argc, argv);
			mpis.serve();
#endif

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
			"-z zht.conf -n neighbor.conf [-p port] [-h(help)]");
}
