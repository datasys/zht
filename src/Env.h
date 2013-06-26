/*
 * Env.h
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef ENV_H_
#define ENV_H_

#include <sys/types.h>
#include <string>
using namespace std;

class Env {
public:
	Env();
	virtual ~Env();

	static int setconfigvariables(string confFile);

public:
	static const int MAX_MSG_SIZE; //max size of a message in each transfer

	static const uint BUF_SIZE; //size of blob transfered from client to server each time

	static const int TOTAL_MSG_SIZE; //total size of a message transfered

	static int REPLICATION_TYPE; //1 for Client-side replication

	static int NUM_REPLICAS;
};

#endif /* ENV_H_ */
