#include <stdio.h>
#include "file_reader.h"
#include "mysql.h"

char command[1024]; // �� ��ɾ�
char line[1024]; // ���� 

void file_reader(const char* file_name) {
	FILE* fp;
	
	if ((fp = fopen(file_name, "r")) == NULL) {
		fprintf(stdout, "file not found!\n");
		exit(1);
	}

	while (fgets(line, 1024, fp) != NULL) {
		if (strcmp(line, "\n") == 0) // ���͸� �Է����� ������ ����
			continue;
		if (line[0] == '#') // �ּ� �Է� ������ ����
			continue;


		//fprintf(stdout, "length: %d\n", strlen(line));
		if (line[strlen(line) - 1] == '\n') // ���Ͱ� ������ �Է��̸�
			line[strlen(line) - 1] = '\0'; // ���� ������ �� ���� ����
		//fprintf(stdout, "line: %s\n", line);
		//fprintf(stdout, "length: %d\n", strlen(line));
		strcat(command, line);// command �� ��ɾ� stack �ױ�
		strcat(command, " "); // �����̽��� �߰�
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


	// ���� ����

	fclose(fp);
}