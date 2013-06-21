/*
 * MsgShared.h
 *
 *  Created on: Jun 19, 2013
 *      Author: Xiaobing Zhou
 */

#ifndef MSGSHARED_H_
#define MSGSHARED_H_

#include <sys/types.h>

struct MsgSrc {

	int fd_src; /*file descriptor of source*/
};

struct MsgDest {

	int fd_dest; /*file descriptor of destination*/
};

struct my_msgbuf {

	long mtype;
	char mtext[200];
};

struct MsgBuf {

	size_t size;
	char *mtext;
};

/*
 *
 */
class MsgShared {
public:
	MsgShared();
	virtual ~MsgShared();

	int get_msqid();

private:
	void init_msg_queue();

private:
	int _msqid;

};

#endif /* MSGSHARED_H_ */
