#include <stdio.h>
#include "mysql.h"
#include "file_reader.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "password";
const char* db = "testing"; //여기다가 원하는 스키마 이름 넣기 

const char* create_table = "curd.txt";
const char* insert_data = "insert.txt";
const char* use_schema = "use testing";

MYSQL* connection = NULL;
MYSQL conn;


int main(void) {



	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, NULL, 3306, (const char*)NULL, 0); //연결하는 함수
	

	// establish connection
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	
	
	printf("Connection Succeed\n");
	// 쿼리 실행
	file_reader(create_table);// read create table
	
	//for(int i = 0; i < ;i++)
	file_reader(insert_data);

	
	mysql_query(connection, "drop SCHEMA testing"); //테이블 폭파
	mysql_close(connection);
	return 0;
}





/*

	printf("------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE 1\n");
	printf("\t2. TYPE 2\n");
	printf("\t3. TYPE 3\n");
	printf("\t4. TYPE 4\n");
	printf("\t5. TYPE 5\n");
	printf("\t6. TYPE 6\n");
	printf("\t7. TYPE 7\n");
	printf("\t0. QUIT\n");
	//printf("----------------------------------\n");
	printf("\n\n");
	printf("---- TYPE 5 ----\n\n");
	printf("** Find the top k brands by unit sales by the year**\n");
	printf(" Which K? : 3\n");

	return 0;

}
*/