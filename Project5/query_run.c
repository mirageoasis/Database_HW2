#include "query_run.h"

const char* prompt = "\
------- SELECT QUERY TYPES ------- \n\n\
\t1. TYPE 1\n\
\t2. TYPE 2\n\
\t3. TYPE 3\n\
\t4. TYPE 4\n\
\t5. TYPE 5\n\
\t6. TYPE 6\n\
\t7. TYPE 7\n\
\t0. QUIT\n\
";

void query_run() {

}


void command_prompt() {

	int user_command_number = 0;

	do{
		fprintf(stdout, prompt);
		fscanf(stdin, "%d", &user_command_number);
	} while (user_command_number != 0);



}
