#include <stdio.h>
#include "file_reader.h"
#include "mysql.h"

char command[1024]; // 총 명령어
char line[1024]; // 한줄 

void file_reader(const char* file_name) {
	FILE* fp;
	
	if ((fp = fopen(file_name, "r")) == NULL) {
		fprintf(stdout, "file not found!\n");
		exit(1);
	}

	while (fgets(line, 1024, fp) != NULL) {
		if (strcmp(line, "\n") == 0) // 엔터만 입력으로 들어오면 무시
			continue;
		if (line[0] == '#') // 주석 입력 들어오면 무시
			continue;


		//fprintf(stdout, "length: %d\n", strlen(line));
		if (line[strlen(line) - 1] == '\n') // 엔터가 마지막 입력이면
			line[strlen(line) - 1] = '\0'; // 엔터 없에고 널 문자 삽입
		//fprintf(stdout, "line: %s\n", line);
		//fprintf(stdout, "length: %d\n", strlen(line));
		strcat(command, line);// command 에 명령어 stack 쌓기
		strcat(command, " "); // 스페이스바 추가
		//fprintf(stdout,"command status: %s\n", command);


		if (line[strlen(line) - 1] == ';') {
			
			if (mysql_query(connection, command) != 0) {
				fprintf(stdout, "command: %s\n", command);
				fprintf(stdout, "invalid command!\n");
				printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			}
			memset(command, 0, sizeof(command));
		}

	}
	//SHOW COLUMNS FROM customer;


	// 쿼리 실행

	fclose(fp);
}