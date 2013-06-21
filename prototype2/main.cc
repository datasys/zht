/*
 *  Copyright (C) 2002 by LifeLine Networks bv.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * \file
 */

#include "ipcplus.h"
#include <time.h>
#include <sys/wait.h>

/// Test program.
int main() {

	// lots of msg tests

	IPC::Msg msg;
	IPC::Msg msg1(1);
	IPC::Msg msg2(1);
	if (msg.Snd("Hello") && msg.Rcv() && msg1.Snd("Hello") && msg2.Rcv()) {
		printf("Succesfully snd and rcv: %s,%s\n", msg.msgbuf.mdata,
				msg2.msgbuf.mdata);
	}
	// priority test
	if (msg.Snd("Me last", 3) && msg.Snd("Me second", 2)
			&& msg.Snd("Me first", 1)) {
		if (msg.RcvHigh()) {
			printf("Priority message: %ld:%s\n", msg.msgbuf.mtype,
					msg.msgbuf.mdata);
		}
		if (msg.RcvHigh()) {
			printf("Priority message: %ld:%s\n", msg.msgbuf.mtype,
					msg.msgbuf.mdata);
		}
		if (msg.RcvHigh()) {
			printf("Priority message: %ld:%s\n", msg.msgbuf.mtype,
					msg.msgbuf.mdata);
		}
	}
	// typed test
	if (msg.Snd("Me last", 3) && msg.Snd("Me second", 2)
			&& msg.Snd("Me first", 1)) {
		if (msg.RcvType(1)) {
			printf("Typed message: %ld:%s\n", msg.msgbuf.mtype,
					msg.msgbuf.mdata);
		}
		if (msg.RcvType(2)) {
			printf("Typed message: %ld:%s\n", msg.msgbuf.mtype,
					msg.msgbuf.mdata);
		}
		if (msg.RcvType(3)) {
			printf("Typed message: %ld:%s\n", msg.msgbuf.mtype,
					msg.msgbuf.mdata);
		}
	}
	msg.destroy();
	msg1.destroy();
	// actual ipc test
	if (!fork()) {
		IPC::Msg s(1);
		char d[512];
		unsigned int n = 0;
		time_t start = time(NULL);
		while (start == time(NULL))
			;
		start = time(NULL);
		while (time(NULL) < start + 4) {
			if (!s.Snd(d, 512, 1)) {
				break;
			}
			n++;
		}
		printf("Send done: %u KByte, %u Kbytes/s\n", n >> 1, n >> 3);
		s.Snd(d, 1, 2);
		exit(0);
	} else {
		IPC::Msg r(1);
		unsigned int n = 0;
		while (r.Rcv()) {
			if (r.msgbuf.mtype == 2) {
				break;
			}
			n++;
		}
		printf("Recv done: %u KByte\n", n >> 1);
		r.destroy();
		wait(NULL);
	}
	if (!fork()) {
		IPC::Msg rt(1);
		time_t t = time(NULL);
		int n = 0;
		while (time(NULL) < t + 6) {
			if (!rt.Snd("1", 1)) {
				abort();
			}
			if (!rt.RcvType(2)) {
				abort();
			}
			n++;
		}
		printf("Round-trip = %.3lf ms (%u ms/s)\n",
				(double) 3000000.0 / (double) n, (n << 1) / 6);
		rt.Snd("Quit", 1);
		exit(0);
	} else {
		IPC::Msg rt(1);
		while (rt.RcvType(1)) {
			if (rt.msgbuf.mdata[0] == 'Q') {
				break;
			}
			rt.Snd("2", 2);
		}
		rt.destroy();
		wait(NULL);
	}

	// client-server test.
	pid_t p1, p2;
	if (0 != (p1 = fork())) {
		if (0 != (p2 = fork())) {
			// server
			IPC::MsgServer s(1);
			char req[20];
			size_t sz;
			int cnt;
			for (cnt = 0; cnt < 1000000; cnt++) {
				if (s.recv(req, sz)) {
					strcat(req, " world");
					s.xmit(req, strlen(req) + 1);
				}
			}
			wait(NULL);
			wait(NULL);
		} else {
			// client 2
			IPC::MsgClient c(1);
			int cnt;
			for (cnt = 0; cnt < 500000; cnt++) {
				c.xmit("Hello 2", 8);
				char ans[20];
				size_t asz;
				c.recv(ans, asz);
				if (strcmp("Hello 2 world", ans)) {
					abort();
				}
			}
			printf("Client 2 done\n");
			exit(0);
		}
	} else {
		// client 1
		IPC::MsgClient c(1);
		int cnt;
		for (cnt = 0; cnt < 500000; cnt++) {
			c.xmit("Hello 1", 8);
			char ans[20];
			size_t asz;
			c.recv(ans, asz);
			if (strcmp("Hello 1 world", ans)) {
				abort();
			}
		}
		printf("Client 1 done\n");
		exit(0);
	}

	return (0);
}

