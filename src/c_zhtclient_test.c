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
 * c_zhtclient_test.c
 *
 *  Created on: Aug 7, 2012
 *      Author: Xiaobingo
 *      Contributor: Tony
 */

#include   <stdbool.h>
#include   <stdlib.h>
#include   <stdio.h>
#include <getopt.h>

#include   <string.h>
#include "c_zhtclient.h"
#include "meta.pb-c.h"

const int LOOKUP_SIZE = 1024 * 10000 * 7; //size of buffer to store lookup result, larger enough than TOTAL_SIZE
//const int LOOKUP_SIZE = 16 * 3; //size of buffer to store lookup result, larger enough than TOTAL_SIZE

const int TOTAL_SIZE = 1024 * 10000 * 6; //total size of a message to be transfered
//const int TOTAL_SIZE = 16 * 2; //total size of a message to be transfered

void test_large_keyvalue();
void test_large_keyvalue_blankspace();
void test_large_keyvalue_emptystring();
void test_large_keyvalue_reuse();

void test_common_usecase();
void test_common_usecase_blankspace();
void test_common_usecase_emptystring();
void test_common_usecase_reuse();

void test_pass_package();
void test_pass_package_blankspace();
void test_pass_package_emptystring();
void test_pass_package_reuse();

void test_simple_largevalue();

void printUsage(char *argv_0);

int main(int argc, char **argv) {

	extern char *optarg;

	int printHelp = 0;
	int numOfOps = -1;
	char *zhtConf = NULL;
	char *neighborConf = NULL;

	int c;
	while ((c = getopt(argc, argv, "z:n:h")) != -1) {
		switch (c) {
		case 'z':
			zhtConf = optarg;
			break;
		case 'n':
			neighborConf = optarg;
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

	if (zhtConf != NULL && neighborConf != NULL ) {

		c_zht_init(zhtConf, neighborConf);

//		test_large_keyvalue();

		test_common_usecase();

		//	test_pass_package();

//		test_simple_largevalue();

		c_zht_teardown();

	} else {

		if (!helpPrinted)
			printUsage(argv[0]);
	}
}

void printUsage(char *argv_0) {

	fprintf(stdout, "Usage:\n%s %s\n", argv_0,
			"-z zht.conf -n neighbor.conf [-h(help)]");
}

void test_simple_largevalue() {

	const char *key = "keyofLargeValue";
//	const char *value = "xiaobingo";

	char *value2 = calloc(TOTAL_SIZE, sizeof(char));
	memset(value2, '1', TOTAL_SIZE - 1);

	/*
	 * test c_zht_insert
	 * */
	int iret = c_zht_insert2(key, value2); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_insert, return code: %d\n", iret);

	/*
	 * test c_zht_lookup
	 * */
	size_t ln;
	char *result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup2(key, result, &ln); //1 for look up, 2 for remove, 3 for insert, 4 for append

		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);
		/*	fprintf(stdout, "c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
		 key, result);*/
	}

	free(result);

	/*
	 * test c_zht_append
	 * */
	int aret = c_zht_append2(key, "value appended"); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_append, return code: %d\n", aret);

	/*
	 * test c_zht_remove
	 * */
	int rret = c_zht_remove2(key); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_remove, return code: %d\n", rret);

	free(value2);
}

void test_common_usecase() {

	test_common_usecase_blankspace();

	fprintf(stdout, "%s\n", "--------------------------");

	test_common_usecase_emptystring();
}

void test_common_usecase_blankspace() {

	const char *key = "hello";
	const char *value = " ";
	const char *value2 = "zht";

	test_common_usecase_reuse(key, value, value2);
}

void test_common_usecase_emptystring() {

	const char *key = "hello";
	const char *value = "";
	const char *value2 = "ZHT";

	test_common_usecase_reuse(key, value, value2);
}

void test_common_usecase_reuse(const char * const key, const char * const value,
		const char * const value2) {

	/*
	 * test c_zht_insert
	 * */
	int iret = c_zht_insert2(key, value); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_insert, return code: %d\n", iret);

	/*
	 * test c_zht_lookup
	 * */
	size_t ln;
	char *result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup2(key, result, &ln); //1 for look up, 2 for remove, 3 for insert, 4 for append

		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);
		fprintf(stdout, "c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
				key, result);
	}

	free(result);

	/*
	 * test c_zht_append
	 * */
	int aret = c_zht_append2(key, value2); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_append, return code: %d\n", aret);

	/*
	 * test c_zht_lookup again
	 * */
	ln = 0;
	result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup2(key, result, &ln); //1 for look up, 2 for remove, 3 for insert, 4 for append

		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);
		fprintf(stdout, "c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
				key, result);
	}

	free(result);

	/*
	 * test c_zht_remove
	 * */
	int rret = c_zht_remove2(key); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_remove, return code: %d\n", rret);
}

void test_large_keyvalue() {

	test_large_keyvalue_blankspace();

	fprintf(stdout, "%s\n", "--------------------------");

	test_large_keyvalue_emptystring();
}

void test_large_keyvalue_blankspace() {

	const char *key = "keyofLargeValue";
	const char *value = " ";

	char *value2 = calloc(TOTAL_SIZE, sizeof(char));
	memset(value2, '1', TOTAL_SIZE - 1);

	test_large_keyvalue_reuse(key, value, value2);

	free(value2);

}

void test_large_keyvalue_emptystring() {

	const char *key = "keyofLargeValue";
	const char *value = "";

	char *value2 = calloc(TOTAL_SIZE, sizeof(char));
	memset(value2, '2', TOTAL_SIZE - 1);

	test_large_keyvalue_reuse(key, value, value2);

	free(value2);
}

void test_large_keyvalue_reuse(const char * const key, const char * const value,
		const char * const value2) {

	/*
	 * test c_zht_insert
	 * */
	int iret = c_zht_insert2(key, value); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_insert, return code: %d\n", iret);

	/*
	 * test c_zht_lookup
	 * */
	size_t ln;
	char *result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup2(key, result, &ln); //1 for look up, 2 for remove, 3 for insert, 4 for append

		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);
		/*fprintf(stdout, "c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
		 key, result);*/
	}

	free(result);

	/*
	 * test c_zht_append
	 * */
	int aret = c_zht_append2(key, value2); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_append, return code: %d\n", aret);

	/*
	 * test c_zht_lookup again
	 * */
	ln = 0;
	result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup2(key, result, &ln); //1 for look up, 2 for remove, 3 for insert, 4 for append

		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);
		/*fprintf(stdout, "c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
		 key, result);*/
	}

	free(result);

	/*
	 * test c_zht_remove
	 */
	int rret = c_zht_remove2(key); //1 for look up, 2 for remove, 3 for insert, 4 for append
	fprintf(stdout, "c_zht_remove, return code: %d\n", rret);

}

void test_pass_package() {

	test_pass_package_blankspace();

	fprintf(stdout, "%s\n", "--------------------------");

	test_pass_package_emptystring();
}

void test_pass_package_blankspace() {

	const char *key = "hello";
	const char *value = " ";
	const char *value2 = "zht";

	test_pass_package_reuse(key, value, value2);
}

void test_pass_package_emptystring() {

	const char *key = "hello";
	const char *value = "";
	const char *value2 = "ZHT";

	test_pass_package_reuse(key, value, value2);
}

void test_pass_package_reuse(const char * const key, const char * const value,
		const char * const value2) {

	char *buf; // Buffer to store serialized data
	unsigned len; // Length of serialized data

	Package package = PACKAGE__INIT; // Package
	package.virtualpath = (char*) key;
	if (strcmp(value, "") != 0) //tricky: bypass protocol-buf's bug
		package.realfullpath = (char*) value;
	package.has_isdir = true;
	package.isdir = false;
	package.has_operation = true;
	package.operation = 3; //1 for look up, 2 for remove, 3 for insert, 4 for append

	len = package__get_packed_size(&package);
	buf = (char*) calloc(len, sizeof(char));
	package__pack(&package, buf);

	/*
	 * test c_zht_insert
	 * */
	int iret = c_zht_insert(buf);
	fprintf(stdout, "c_zht_insert, return code: %d\n", iret);
	free(buf);

	/*
	 * test c_zht_lookup
	 * */
	Package package2 = PACKAGE__INIT;
	package2.virtualpath = (char*) key;
	package2.operation = 1; //1 for look up, 2 for remove, 3 for insert, 4 for append

	len = package__get_packed_size(&package2);
	buf = (char*) calloc(len, sizeof(char));
	package__pack(&package2, buf);

	size_t ln;
	char *result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup(buf, result, &ln);
		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);

		if (lret == 0 && ln > 0) {

			Package * lPackage;
			char *lBuf = (char*) calloc(ln, sizeof(char));

			strncpy(lBuf, result, ln);
			lPackage = package__unpack(NULL, ln, lBuf);

			if (lPackage == NULL ) {

				fprintf(stdout, "error unpacking lookup result\n");

			} else {

				fprintf(stdout,
						"c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
						lPackage->virtualpath, lPackage->realfullpath);

			}

			free(lBuf);
			package__free_unpacked(lPackage, NULL );
		}
	}

	free(buf);
	free(result);

	/*
	 * test c_zht_append
	 * */
	Package package4 = PACKAGE__INIT;
	package4.virtualpath = (char*) key;
	if (strcmp(value2, "") != 0) //tricky: bypass protocol-buf's bug
		package4.realfullpath = (char*) value2;
	package4.has_isdir = true;
	package4.isdir = false;
	package4.has_operation = true;
	package4.operation = 4; //1 for look up, 2 for remove, 3 for insert, 4 for append

	len = package__get_packed_size(&package4);
	buf = (char*) calloc(len, sizeof(char));
	package__pack(&package4, buf);

	int aret = c_zht_append(buf);
	fprintf(stdout, "c_zht_append, return code: %d\n", aret);
	free(buf);

	/*
	 * test c_zht_lookup again
	 * */
	Package package5 = PACKAGE__INIT; // Package
	package5.virtualpath = (char*) key;
	package5.operation = 1; //1 for look up, 2 for remove, 3 for insert, 4 for append

	len = package__get_packed_size(&package5);
	buf = (char*) calloc(len, sizeof(char));
	package__pack(&package5, buf);

	ln = 0;
	result = (char*) calloc(LOOKUP_SIZE, sizeof(char));

	if (result != NULL ) {

		int lret = c_zht_lookup(buf, result, &ln);
		fprintf(stdout, "c_zht_lookup, return code(length): %d(%lu)\n", lret,
				ln);

		if (lret == 0 && ln > 0) {

			Package * lPackage;
			char *lBuf = (char*) calloc(ln, sizeof(char));

			strncpy(lBuf, result, ln);
			lPackage = package__unpack(NULL, ln, lBuf);

			if (lPackage == NULL ) {

				fprintf(stdout, "error unpacking lookup result\n");

			} else {

				fprintf(stdout,
						"c_zht_lookup, return {key}:{value} => {%s}:{%s}\n",
						lPackage->virtualpath, lPackage->realfullpath);

			}

			free(lBuf);
			package__free_unpacked(lPackage, NULL );
		}
	}

	free(buf);
	free(result);

	/*
	 * test c_zht_remove
	 * */
	Package package3 = PACKAGE__INIT;
	package3.virtualpath = (char*) key;
	package3.operation = 2; //1 for look up, 2 for remove, 3 for insert, 4 for append

	len = package__get_packed_size(&package3);
	buf = (char*) calloc(len, sizeof(char));
	package__pack(&package3, buf);

	int rret = c_zht_remove(buf);
	fprintf(stdout, "c_zht_remove, return code: %d\n", rret);
	free(buf);
}
