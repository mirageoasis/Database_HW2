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
// 1�� ���� �޴� �Է�(X)
// 1-1�� online_sales�� �Է� 1. �׳� �Ǵ� ��� / 2. �ȵż� ���ֹ� �ִ� ���̽�

/// <summary>
///  1�� �ó����� �ı��Ǹ� arrived time�� 0 �� �־� ������ // �ֹ��� ���α׷����� �ٽ� ������ ����
/// </summary>

const char* type_1_query[] = {
"\
	SELECT customer_id\n\
	FROM online_sales\n\
	WHERE tracking_number=%d;\
	",

"\
	SELECT email_address\n\
	FROM customer\n\
	WHERE customer_id=%d;\
"
};

const char* type_1_1_query[] = {
	"SELECT ",
	""
};

// 2�� select ��
// 2-1 select ��

// 3�� select �� -> online in_store���� column �������� union ���Ŀ� distinct �� �ѹ� ������ ��
// 3-1 �� select ��
// 3-2 �� select �� 


// 4�� select �� 
// 4-1 �� select �� ���� ���� (k)
// 4-2 �� select �� 

// 5�� select ��
// 6�� select ��
// 7�� selec ��

char command[1024];

int subquery_command_list[4] = {1, 2, 3, 4 };
int number_of_subquery[4] = {1, 1, 2, 2};

//1,2,3, 4�� ���� query_result set�� �����ϰ� �ִٰ�

// sub ��ɾ ����Ǹ� �� ��ø ���� ���� ���ڸ� ���� �ֱ� ����?


void run_query(int command_number) {
	if (!command_number)
		return;
	assert(command_number != 0);
	
	mysql_query(connection, use_schema);

	if (command_number == 1) {
		
		int tracking_number;
		fprintf(stdout, "which is X? : ");
		fscanf(stdin, "%d", &tracking_number);

		sprintf(command, type_1_query[0], tracking_number); // customer id ���� ã��

		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		}
	
		MYSQL_ROW* customer_id_string = mysql_fetch_row(mysql_store_result(connection));// ���� �����ϰ� ��������
		// �� ã�ƾ��Ѵ� ������ �ݺ������� 
		int customer_id = atoi(customer_id_string[0]);
		// �� ã�ƾ��Ѵ� ������ �ݺ������� 

		//fprintf(stdout, "customer id is: %s\n", customer_id_string[0]);
		fprintf(stdout, "customer id is: %d\n", customer_id);
		
		assert(customer_id != 0);
		//strtol ���? ���� ��� ��į?
		sprintf(command, type_1_query[1], customer_id); // ���� ���� �ϼ�
		
		//fprintf(stdout, "command: %s\n", command);

		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		}

		MYSQL_ROW* email_string = mysql_fetch_row(mysql_store_result(connection));// ���� �����ϰ� ��������
		// �ڷῡ�� tracking number �� ���� customer �� ���� ���� 
		fprintf(stdout, "%s\n", email_string[0]);
		// �� ã�ƾ��Ѵ� ������ �ݺ������� 
	}
	// �Լ��� �и��ϱ�

	return;
}

int if_subquery(int target) {
	for (int i = 0; i < sizeof(subquery_command_list) / sizeof(int); i++)
		if (target == subquery_command_list[i])
			return subquery_command_list[i];

	return 0;
}

void run_subquery() {
}

void command_prompt() {

	int user_command_number = 0;

	while (1){
		fprintf(stdout, prompt);
		fscanf(stdin, "%d", &user_command_number);
		
		if (user_command_number == 0)
			break;

		assert(user_command_number != 0);

		run_query(user_command_number);

		run_subquery(if_subquery(user_command_number));

	}

}
