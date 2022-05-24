
#ifndef file_reader_H
#define file_reader_H
#include "mysql.h"

extern MYSQL *connection;
extern MYSQL conn;

void file_reader(const char* file_name);

#endif