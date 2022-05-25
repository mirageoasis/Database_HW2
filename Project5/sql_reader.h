
#ifndef sql_reader_H
#define sql_reader_H
#include "mysql.h"

extern MYSQL *connection;
extern MYSQL conn;

void file_reader(const char* file_name);

#endif