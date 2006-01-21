/* 
 * Copyright (C) 2005 Michael J Jones, mrmikejj at hotmail dot com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "../client/stdinc.h"
#include "../client/DCPlusPlus.h"

#include "Fdm-UserDatabase.h"

#include "../client/Util.h"
#include "../client/File.h"

#include "../other-projects/CppSQLite/CppSQLite3.h"

using namespace std;

const string userDataFile = Util::getAppPath() + "user.db";  
const char* userDatabase = userDataFile.c_str();  

void FdmUserDatabase::createUserDB() {
	//try {
		CppSQLite3DB db;
		removeUserDB();
		db.open(userDatabase);
		// add tables
		db.execDML("create table user(dbuserid integer primary key, nick varchar(64));");
	//	db.execDML("create table emp(empno int, empname varchar(20));"); //CREATE TABLE 
		db.execDML("begin transaction;");
		char buf[128];
		for (int i = 0; i < 10000; i++) {
			sprintf(buf, "insert into user (nick) values ('Empname%06d');", i);
			db.execDML(buf);
		}
		db.execDML("commit transaction;");
		db.close();
	//} catch(...) {
	//}
}

void FdmUserDatabase::removeUserDB() {
	if (File::getSize(userDatabase) != -1)
		remove(userDatabase);
}