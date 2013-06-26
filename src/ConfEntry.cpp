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
 * ConfEntry.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: tony, xiaobingo
 */

#include "ConfEntry.h"

#include "Const-impl.h"

#include <stdio.h>
#include <string.h>

namespace iit {
namespace datasys {
namespace zht {
namespace dm {

ConfEntry::ConfEntry() {

}

ConfEntry::ConfEntry(const string& sconfigEntry) {

	assign(sconfigEntry);
}

ConfEntry::ConfEntry(const string& name, const string& value) :
		_name(name), _value(value) {

}

ConfEntry::~ConfEntry() {

}

string ConfEntry::name() const {

	return _name;
}

void ConfEntry::name(const string& name) {

	_name = name;
}

string ConfEntry::value() const {

	return _value;
}

void ConfEntry::value(const string& value) {

	_value = value;
}

string ConfEntry::operator()() const {

	return toString();
}

string ConfEntry::toString() const {

	char buf[50];
	memset(buf, 0, sizeof(buf));
	int n = sprintf(buf, getFormat().c_str(), _name.c_str(), _value.c_str());

	string result(buf, 0, n);

	return result;
}

ConfEntry& ConfEntry::assign(string sconfigEntry) {

	const char* delimiter = ",";

	string remains = Const::trim(sconfigEntry);

	size_t found = remains.find(delimiter);

	if (found != string::npos) {

		name(Const::trim(remains.substr(0, int(found))));
		value(Const::trim(remains.substr(int(found) + 1)));
	}

	return *this;
}

string ConfEntry::getFormat() {

	return "%s,%s";
}

} /* namespace dm */
} /* namespace zht */
} /* namespace datasys */
} /* namespace iit */
