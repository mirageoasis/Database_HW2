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
// 1번 변수 받는 입력(X)
// 1-1은 online_sales에 입력 1. 그냥 되는 경우 / 2. 안돼서 재주문 넣는 케이스

/// <summary>
///  1번 시나리오 파괴되면 arrived time에 0 값 넣어 버리기 // 주문은 프로그램으로 다시 만들자 ㅇㅇ
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

// 2번 select 문
// 2-1 select 문

// 3번 select 문 -> online in_store에서 column 가져오고 union 이후에 distinct 문 한번 돌리면 끝
// 3-1 번 select 문
// 3-2 번 select 문 


// 4번 select 문 
// 4-1 번 select 문 변수 받은 (k)
// 4-2 번 select 문 

// 5번 select 문
// 6번 select 문
// 7번 selec 문

char command[1024];

int subquery_command_list[4] = {1, 2, 3, 4 };
int number_of_subquery[4] = {1, 1, 2, 2};

//1,2,3, 4의 경우는 query_result set을 저장하고 있다가

// sub 명령어가 수행되면 걍 중첩 쿼리 만들어서 숫자만 집어 넣기 ㄱㄱ?


void run_query(int command_number) {
	if (!command_number)
		return;
	assert(command_number != 0);
	
	mysql_query(connection, use_schema);

	if (command_number == 1) {
		
		int tracking_number;
		fprintf(stdout, "which is X? : ");
		fscanf(stdin, "%d", &tracking_number);

		sprintf(command, type_1_query[0], tracking_number); // customer id 쿼리 찾기

		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		}
	
		MYSQL_ROW* customer_id_string = mysql_fetch_row(mysql_store_result(connection));// 쿼리 저장하고 가져오기
		// 다 찾아야한다 ㅋㅋㅋ 반복문으로 
		int customer_id = atoi(customer_id_string[0]);
		// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

		//fprintf(stdout, "customer id is: %s\n", customer_id_string[0]);
		fprintf(stdout, "customer id is: %d\n", customer_id);
		
		assert(customer_id != 0);
		//strtol 사용? 없는 경우 어캄?
		sprintf(command, type_1_query[1], customer_id); // 쿼리 최종 완성
		
		//fprintf(stdout, "command: %s\n", command);

		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		}

		MYSQL_ROW* email_string = mysql_fetch_row(mysql_store_result(connection));// 쿼리 저장하고 가져오기
		// 자료에서 tracking number 는 여러 customer 에 적용 가능 
		fprintf(stdout, "%s\n", email_string[0]);
		// 다 찾아야한다 ㅋㅋㅋ 반복문으로 
	}
	// 함수로 분리하기

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
