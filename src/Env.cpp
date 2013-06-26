/*
 * Env.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: Xiaobing Zhou
 */

#include "Env.h"

//const int Env::MAX_MSG_SIZE = 1024 * 2; //max size of a message in each transfer
const int Env::MAX_MSG_SIZE = 65535; //max size of a message in each transfer

const uint Env::BUF_SIZE = 512 + 38;

const int Env::TOTAL_MSG_SIZE = 1024 * 1024 * 10; //total size of a message transfered, 10M

int Env::REPLICATION_TYPE = 0; //1 for Client-side replication

int Env::NUM_REPLICAS = 0;

Env::Env() {
}

Env::~Env() {
}
