#ifndef query_run_H
#define query_run_H

#include <stdio.h>
#include "mysql.h"

extern MYSQL* connection;
extern MYSQL conn;
extern const char* use_schema;

void command_prompt();

#endif
