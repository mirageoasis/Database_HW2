#ifndef query_run_H
#define query_run_H

#include <stdio.h>
#include <assert.h>
#include "mysql.h"

extern MYSQL* connection;
extern MYSQL conn;
extern const char* use_schema;

void command_prompt();
void run_query(int command_number);
void run_subquery();
int if_subquery(int target);


#endif
