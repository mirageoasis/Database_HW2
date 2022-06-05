#include "query_run.h"

const char* prompt[] = {
"\
------- SELECT QUERY TYPES ------- \n\n\
\t1. TYPE 1\n\
\t2. TYPE 2\n\
\t3. TYPE 3\n\
\t4. TYPE 4\n\
\t5. TYPE 5\n\
\t6. TYPE 6\n\
\t7. TYPE 7\n\
\t0. QUIT\n\
"
,
"\
------- Subtypes in TYPE 1 ------- \n\
\t1. TYPE 1-1\n\
"
,
"\
------- Subtypes in TYPE 2 ------- \n\
\t1. TYPE 2-1\n\
"
,
"\
------- Subtypes in TYPE 3 ------- \n\
\t1. TYPE 3-1\n\
\t2. TYPE 3-2\n\
"
,
"\
------- Subtypes in TYPE 4 ------- \n\
\t1. TYPE 4-1\n\
\t2. TYPE 4-2\n\
"
};
// 1번 변수 받는 입력(X)
// 1-1은 online_sales에 입력 1. 그냥 되는 경우 / 2. 안돼서 재주문 넣는 케이스

/// <summary>
///  1번 시나리오 파괴되면 arrived time에 0 값 넣어 버리기 // 주문은 프로그램으로 다시 만들자 ㅇㅇ
/// </summary>

const char* type_1_query[] = {
"\
	SELECT * \n\
	FROM online_sales\n\
	WHERE tracking_number=%d;\
"
,
"\
	SELECT email_address\n\
	FROM customer\n\
	WHERE customer_id in (%s);\
"

};

const char* type_1_1_query[] = {
"\
	SELECT *\n\
	FROM tracking_id\n\
	WHERE tracking_number=%d;\
"
,
"\
	INSERT INTO tracking_id(tracking_number, ETA, time_arrived)\n\
	VALUES(%d, '%s', '%s');\
"
,
"\
	SELECT *\n\
	FROM online_sales\n\
	WHERE tracking_number=%d;\
"	
,
"\
	INSERT INTO online_sales(order_time, card_number, amount, tracking_number, product_id, customer_id, warehouse_id)\n\
	VALUES\n\
"
,
"\
	('%s', '%s', '%s', %d, '%s', '%s', '%s'),\n\
"
};

// 2번 select 문
// 2-1 select 문

const char* type_2_query[] =  {
"\
	SELECT A.customer_id, customer.name, SUM(A.tot)\n\
	FROM customer natural join (\n\
	SELECT customer_id ,product_id, sum(price * amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY customer_id \n\
	UNION ALL\n\
	SELECT customer_id , product_id, sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY customer_id \n\
	) AS A\
	GROUP BY A.customer_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	LIMIT 1;\n\
",
"\
	SELECT customer_id , sum(price* amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) < DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY customer_id \n\
	UNION ALL\n\
	SELECT customer_id , sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time)<DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY customer_id \n\
	;\n\
"
};

const char* type_2_1_query[] = {
"\
	SELECT A.customer_id, customer.name, N, A.tot\n\
	FROM customer natural join (\n\
	SELECT customer_id ,product_id, name AS N, sum(amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00') AND customer_id=%d\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT customer_id , product_id, name AS N, sum(amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00') AND customer_id=%d\n\
	GROUP BY product_id \n\
	) AS A\
	WHERE A.customer_id = %d\n\
	ORDER BY A.tot DESC\n\
	LIMIT 1;\n\
",
"\
	SELECT customer_id ,product_id, sum(amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id\n\
	UNION ALL\n\
	SELECT customer_id , product_id, sum(amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id\n\
	;\n\
"
};

// 3번 select 문 -> online in_store에서 column 가져오고 union 이후에 distinct 문 한번 돌리면 끝
// 3-1 번 select 문
// 3-2 번 select 문 
const char* type_3_query[] = {
"\
	SELECT A.name, SUM(A.tot)\n\
	FROM (\n\
	SELECT name ,product_id, sum(price* amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT name, product_id, sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	) AS A\
	GROUP BY A.product_id\n\
	ORDER BY SUM(A.tot) DESC\n\
"
};

const char* type_3_1_query[] = {
"\
	SELECT A.name, SUM(A.tot)\n\
	FROM (\n\
	SELECT name ,product_id, sum(price* amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT name, product_id, sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	) AS A\
	GROUP BY A.product_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	LIMIT %d;\n\
"
};

const char* type_3_2_query[] = {
"\n\
	SELECT B.N, B.P, B.T, B.per_rank * 100\n\
	FROM(SELECT A.name AS N, A.product_id AS P, SUM(A.tot) AS T,PERCENT_RANK() OVER(ORDER BY SUM(A.tot) DESC) as per_rank \n\
	FROM\n\
	(\n\
	SELECT name ,product_id, sum(price* amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT name, product_id, sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	) AS A\n\
	GROUP BY A.product_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	)\n\
	AS B\n\
	WHERE B.per_rank <= 0.1;\n\
"
};

// 4번 select 문 
// 4-1 번 select 문 변수 받은 (k)
// 4-2 번 select 문 

const char* type_4_query[] = {
"\
	SELECT A.name, A.product_id, SUM(A.tot)\n\
	FROM (\n\
	SELECT name ,product_id, sum(amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT name, product_id, sum(amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	) AS A\
	GROUP BY A.product_id\n\
	ORDER BY SUM(A.tot) DESC\n\
"
,	
"\
	SELECT product_id, sum(price * amount)\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time)<DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	;\n\
"
,
"\
	SELECT product_id, sum(price * amount)\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time)<DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	;\n\
"
};

const char* type_4_1_query[] = {
"\
	SELECT A.name, SUM(A.tot)\n\
	FROM (\n\
	SELECT name ,product_id, sum(amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT name, product_id, sum(amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	) AS A\
	GROUP BY A.product_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	LIMIT %d;\n\
",
"\n\
	SELECT * FROM(\n\
	SELECT total_bill,\n\
	PERCENT_RANK()\n\
	OVER(\n\
	ORDER BY total_bill DESC) as per_rank \n\
	FROM\n\
	tips) a\n\
	WHERE a.per_rank <= 0.01;\n\
"
};

const char* type_4_2_query[] = {
"\n\
	SELECT B.N, B.P, B.T, B.per_rank * 100\n\
	FROM(SELECT A.name AS N, A.product_id AS P, SUM(A.tot) AS T,PERCENT_RANK() OVER(ORDER BY SUM(A.tot) DESC) as per_rank \n\
	FROM\n\
	(\n\
	SELECT name ,product_id, sum(amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	UNION ALL\n\
	SELECT name, product_id, sum(amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 YEAR, '%%Y-01-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-01-01 00:00:00')\n\
	GROUP BY product_id \n\
	) AS A\n\
	GROUP BY A.product_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	)\n\
	AS B\n\
	WHERE B.per_rank <= 0.1;\n\
"
};

// 5번 select 문

const char* type_5_query[] = {
"\
	SELECT product_id, name\n\
	FROM product \n\
	WHERE product.product_id NOT IN\n\
	(\n\
	SELECT DISTINCT product_id\n\
	FROM store_stock NATURAL JOIN store\n\
	WHERE store_stock.amount > 0 AND store.region='California'\n\
	)\n\
	;\n\
"
};


// 6번 select 문
const char* type_6_query[] = {
"\
	SELECT online_sales.order_id, tracking_id.tracking_number,tracking_id.ETA, tracking_id.time_arrived\n\
	FROM online_sales NATURAL JOIN tracking_id\n\
	WHERE (tracking_id.ETA < tracking_id.time_arrived) OR (current_timestamp() > tracking_id.ETA AND tracking_id.time_arrived='0000-00-00 00:00:00')\n\
	ORDER BY order_id\n\
	;\n\
"
};

const char* type_7_query[] = {
"\
	SELECT A.customer_id, customer.name, SUM(A.tot)\n\
	FROM customer natural join (\n\
	SELECT customer_id ,product_id, sum(price* amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 MONTH, '%%Y-%%m-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-%%m-01 00:00:00')\n\
	GROUP BY customer_id \n\
	UNION ALL\n\
	SELECT customer_id , product_id, sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE  DATE(order_time) BETWEEN DATE_FORMAT(NOW() - INTERVAL 1 MONTH, '%%Y-%%m-01 00:00:00') AND DATE_FORMAT(NOW(), '%%Y-%%m-01 00:00:00')\n\
	GROUP BY customer_id \n\
	) AS A\
	WHERE A.customer_id IN (SELECT customer_id FROM contract)\n\
	GROUP BY A.customer_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	;\n\
",
"\
	SELECT A.customer_id, customer.name, SUM(A.tot)\n\
	FROM customer natural join (\n\
	SELECT customer_id ,product_id, sum(price* amount) AS tot\n\
	FROM online_sales natural join product \n\
	WHERE DATE(order_time) < DATE_FORMAT(NOW(), '%%Y-%%m-01 00:00:00')\n\
	GROUP BY customer_id \n\
	UNION ALL\n\
	SELECT customer_id , product_id, sum(price * amount) AS tot\n\
	FROM in_store_sales natural join product \n\
	WHERE DATE(order_time)<DATE_FORMAT(NOW(), '%%Y-%%m-01 00:00:00')\n\
	GROUP BY customer_id \n\
	) AS A\
	GROUP BY A.customer_id\n\
	ORDER BY SUM(A.tot) DESC\n\
	;\n\
",
};

// 7번 selec 문 union 을 사용하는 거로

char command[1024];

int subquery_command_list[4] = { 1, 2, 3, 4 };
int number_of_subquery[4] = { 1, 1, 2, 2 };

//1,2,3, 4의 경우는 query_result set을 저장하고 있다가

// sub 명령어가 수행되면 걍 중첩 쿼리 만들어서 숫자만 집어 넣기 ㄱㄱ?

void command_type_1_1_function(int broken_tracking_number, int numbers) {
	
	fprintf(stdout, "------TYPE 1-1  ---------\n\n\n\n");


	int new_tracking_number;
	srand(time(NULL));
	while (1) {
		new_tracking_number = rand();
		sprintf(command, type_1_1_query[0], new_tracking_number); // customer id 쿼리 찾기
		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		}
		MYSQL_RES* tracking_result = mysql_use_result(connection);

		while (mysql_fetch_row(tracking_result)) {
		}
		// 해당 sql 이 정보를 가지고 있는지 테스트 없다면 break문을 수행

		if (!mysql_num_rows(tracking_result)) { // 없으면 그거로 tracking_number 결정
			break;
		}
	}

	// 시간 새로 만드는 중
	time_t rawtime; // 현재 시간 받아줄 구조체
	
	struct tm* new_ETA_tm;
	struct tm* now_tm;
	char new_ETA_char[20] = "";
	char now_char[20] = "";

	time(&rawtime);
	now_tm = localtime(&rawtime);
	sprintf(now_char, "%d-%02d-%02d %02d:%02d:%02d",now_tm->tm_year + 1900, now_tm->tm_mon, now_tm->tm_mday, now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);
	sprintf(new_ETA_char, "%d-%02d-%02d %02d:%02d:%02d", now_tm->tm_year + 1900 , (now_tm->tm_mon + 2) % 12, now_tm->tm_mday, 0, 0, 0);
	//fprintf(stdout, "now clock is %s\n", now_char);
	//fprintf(stdout, "new ETA is %s\n", new_ETA_char);
	sprintf(command, type_1_1_query[1], new_tracking_number, new_ETA_char,"0000-00-00 00:00:00");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}


	// command 설정하기
	sprintf(command, type_1_1_query[2], broken_tracking_number);
	// 1번 쿼리문 그대로 실행 망가진 택배 찾기
	
	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		char customer_list_char[1024];
		int count = 0;

		struct tm temp[3];

		memset(customer_list_char, 0, sizeof(customer_list_char));

		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함


		memset(command, 0, sizeof(command));
		strcat(command, type_1_1_query[3]);

		while ((row = mysql_fetch_row(result_first))) {
			//  이거 고쳐야함 	1번 고치고 new_tracking_number
			//row [1]을 새로 넣기
			sprintf(command + strlen(command), type_1_1_query[4], now_char, row[2], row[3], new_tracking_number,row[5], row[6], row[7]); // 커맨드에 insert into 넣기
			fprintf(stdout, "created new order tracking number of %d\n", new_tracking_number);
			 //  쿼리 실행

		}
		fprintf(stdout, "\n\n\n");
		//2022-05-14 00:00:00 

		command[strlen(command) - 2] = ';';

		//printf("%s\n", command);

		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		}

		MYSQL_RES* result_second = mysql_use_result(connection);
		result_second = mysql_use_result(connection);
		result_second = mysql_use_result(connection);


		mysql_free_result(result_second);

		//while ((row = mysql_fetch_row(result_second))) {
		//	// 이거해야 결과가 입력이 된다.
		//}
		//if (!mysql_num_rows(result_second)) {
		//	fprintf(stdout, "no result\n");
		//	//mysql_free_result(result_first);
		//	return;
		//}// 결과가 존재하지 않으면 알림 띄우기
		//mysql_free_result(result_first);
	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}
}

void command_type_1_function() {

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


	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		int result_num = 0;
		char customer_list_char[1024];
		memset(customer_list_char, 0, sizeof(customer_list_char));
		
		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "mysql_num_result %s\n", row[8]); // row[8] 이 customer_id column의 개수 만큼 결과가 나온다.
			
			assert(atoi(row[0]) != 0);
			result_num++;
			sprintf(customer_list_char + strlen(customer_list_char), "%s,", row[6]);
		}
		
		if (!mysql_num_rows(result_first)) {
			fprintf(stdout, "no result\n", mysql_num_rows(result_first));
			return;
		}// 결과가 존재하지 않으면 함수 중단

		customer_list_char[strlen(customer_list_char) - 1] = '\0'; // 마지막에 쉼표 없에기

		sprintf(command, type_1_query[1], customer_list_char); // 쿼리 최종 완성
		
		//fprintf(stdout, "최종 쿼리 %s\n", command);
		
		// 고객 id 받아오고 email도 받아올 차례


		if (mysql_query(connection, command) != 0) {
			fprintf(stdout, "command: %s\n", command);
			fprintf(stdout, "invalid command!\n");
			printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return;
		} //  쿼리 실행

		MYSQL_RES* result_second = mysql_use_result(connection); // 받아온 email 결과를 use 한다는 선언

		while ((row = mysql_fetch_row(result_second))) {
			/// 반복문 돌면서 email출력
			fprintf(stdout, "email for customer is %s\n", row[0]);
		}
		fprintf(stdout, prompt[1]);

		int to_subquery = 0;
		fscanf(stdin, "%d", &to_subquery);

		if (to_subquery == 1)
			command_type_1_1_function(tracking_number, result_num);
		else
			fprintf(stdout, "invalid command!\n\n\n");
	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_2_1_function(int customer_id) {
	sprintf(command, type_2_1_query[0], customer_id, customer_id, customer_id); // customer id 쿼리 찾기
	fprintf(stdout, "------------ TYPE 2-1 ------------\n\n\n");
	fprintf(stdout, "------------Best product that vip bought most last year!------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}
	MYSQL_RES* result_first = mysql_use_result(connection);

	if (result_first) { // if there are error in query

		MYSQL_ROW row;

		while ((row = mysql_fetch_row(result_first))) {
			fprintf(stdout, "customer '%s' bought '%s' units of '%s' last year!\n", row[1], row[3], row[2]);
		}

		fprintf(stdout, "\n\n------------Best product that vip bought most last year!------------\n\n\n");

	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;



}

void command_type_2_function() {
	sprintf(command, type_2_query[0]); // customer id 쿼리 찾기

	fprintf(stdout, "------------Best customer of last year!------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}
	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		int customer_id = 0;

		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "mysql_num_result %s\n", row[8]); // row[8] 이 customer_id column의 개수 만큼 결과가 나온다.
			//fprintf(stdout, "%s %s %s\n", row[0], row[1], row[2]);
			customer_id = atoi(row[0]);
			fprintf(stdout, "customer %s used %s$\n", row[1], row[2]);
		}

		fprintf(stdout, "\n\n------------Best customer of last year!------------\n\n\n");

		//  쿼리 실행


		fprintf(stdout, prompt[2]);

		int to_subquery = 0;
		fscanf(stdin, "%d", &to_subquery);

		if (to_subquery == 1) {
			//fprintf(stdout, "number 1!\n");
			assert(customer_id != 0);
			command_type_2_1_function(customer_id);
		}
		else {
			fprintf(stdout, "invalid command!\n");
			return;
		}

	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_3_1_function() {

	/// <summary>
	/// 최종 결과
	/// </summary>
	fprintf(stdout, "------------ TYPE 3-1 ------------\n\n\n");
	fprintf(stdout, "** Then find the top k produces by dollar amount sales **\nWhich is K? : ");

	int k;
	fscanf(stdin, "%d", &k);

	sprintf(command, type_3_1_query[0], k); // customer id 쿼리 찾기

	fprintf(stdout, "------------top %d products------------\n\n\n", k);

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);

	if (result_first) { // if there are error in quer
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result_first))) {
			fprintf(stdout, "%s %s\n", row[0], row[1]);
		}
		fprintf(stdout, "\n\n------------top %d products------------\n\n\n", k);


	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_3_2_function() {

	/// <summary>
	/// 최종 결과
	/// </summary>

	//fprintf(stdout, "** Then find the top k produces by dollar amount sales **\nWhich is K? : ");
	fprintf(stdout, "------------ TYPE 3-2 ------------\n\n\n");

	sprintf(command, type_3_2_query[0]); // customer id 쿼리 찾기

	//fprintf(stdout, "------------top %d products------------\n\n\n", k);

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);

	if (result_first) { // if there are error in quer
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "%s %s %s %.3s%%\n", row[0], row[1], row[2], row[3]);		
			fprintf(stdout, "%s %s %.3s%%\n", row[0], row[2], row[3]);
		}
		//fprintf(stdout, "\n\n------------top %d products------------\n\n\n", k);


	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_3_function() {

	sprintf(command, type_3_query[0]); // customer id 쿼리 찾기

	fprintf(stdout, "------------list of products sold last year!------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		int result_num = 0;

		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "mysql_num_result %s\n", row[8]); // row[8] 이 customer_id column의 개수 만큼 결과가 나온다.

			
			fprintf(stdout, "%s %s\n", row[0], row[1]);
		}

		fprintf(stdout, "\n\n------------list of products sold last year!------------\n\n\n");

		//  쿼리 실행
		
		
		fprintf(stdout, prompt[3]);

		int to_subquery = 0;
		fscanf(stdin, "%d", &to_subquery);

		if (to_subquery == 1) {
			//fprintf(stdout, "number 1!\n");
			command_type_3_1_function();
		}
		else if (to_subquery == 2) {
			//fprintf(stdout, "number 2!\n");
			command_type_3_2_function();
		}
		else {
			fprintf(stdout, "invalid command!\n");
			return;
		}


	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_4_1_function() {

	/// <summary>
	/// 최종 결과
	/// </summary>
	fprintf(stdout, "------------ TYPE 4-1 ------------\n\n\n");
	fprintf(stdout, "** Then find the top k products by unit sales **\nWhich is K? : ");

	int k;
	fscanf(stdin, "%d", &k);

	sprintf(command, type_4_1_query[0], k); // customer id 쿼리 찾기

	fprintf(stdout, "------------top %d products------------\n\n\n", k);

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);

	if (result_first) { // if there are error in quer
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result_first))) {
			fprintf(stdout, "%s %s\n", row[0], row[1]);
		}
		fprintf(stdout, "\n\n------------top %d products------------\n\n\n", k);


	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_4_2_function() {

	/// <summary>
	/// 최종 결과
	/// </summary>
	//34번 고객으로 테스트 가능함
	//fprintf(stdout, "** Then find the top k produces by dollar amount sales **\nWhich is K? : ");
	fprintf(stdout, "------------ TYPE 4-2 ------------\n\n\n");
	fprintf(stdout, "** Found the top 10%% products by unit sales **\n\n");

	sprintf(command, type_4_2_query[0]); // customer id 쿼리 찾기

	//fprintf(stdout, "------------top %d products------------\n\n\n", k);

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);

	if (result_first) { // if there are error in quer
		MYSQL_ROW row;
		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "%s %s %s %.3s%%\n", row[0], row[1], row[2], row[3]);
			fprintf(stdout, "%s %s %.3s%%\n", row[0], row[2], row[3]);
		}
		//fprintf(stdout, "\n\n------------top %d products------------\n\n\n", k);


	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_4_function() {

	sprintf(command, type_4_query[0]); // customer id 쿼리 찾기

	fprintf(stdout, "------------list of products sold last year! unit sales------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		int result_num = 0;

		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			fprintf(stdout, "%s %s\n", row[0], row[2]);
			//fprintf(stdout, "%s %s %s\n", row[0], row[1], row[2]);
		}

		fprintf(stdout, "\n\n------------list of products sold last year! unit sales------------\n\n\n");

		//  쿼리 실행


		fprintf(stdout, prompt[4]);

		int to_subquery = 0;
		fscanf(stdin, "%d", &to_subquery);

		if (to_subquery == 1) {
			//fprintf(stdout, "number 1!\n");
			command_type_4_1_function();
		}
		else if (to_subquery == 2) {
			//fprintf(stdout, "number 2!\n");
			command_type_4_2_function();
		}
		else {
			fprintf(stdout, "invalid command!\n");
			return;
		}


	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_5_function() {

	
	sprintf(command, type_5_query[0]); // customer id 쿼리 찾기
	fprintf(stdout, "------------TYPE 5------------\n\n\n");
	fprintf(stdout, "------------products out of stock at every store in California------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		int result_num = 0;

		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "%s\n", row[0]);
			fprintf(stdout, "product '%s' is out of stock\n", row[1]);
			//fprintf(stdout, "%s %s %s %s\n", row[0], row[1], row[2], row[3]);
		}

		fprintf(stdout, "\n\n------------products out of stock at every store in California------------\n\n\n");

		//  쿼리 실행

	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_6_function() {


	sprintf(command, type_6_query[0]); // customer id 쿼리 찾기
	fprintf(stdout, "------------TYPE 6------------\n\n\n");
	fprintf(stdout, "------------these pacakages were not delivered in time------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		int result_num = 0;

		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "%s\n", row[0]);
			if(strcmp(row[3], "0000-00-00 00:00:00") == 0) // 아직도 도착 안한 케이스
				fprintf(stdout, "order '%03s' late on arrival! ETA was %.10s but has not arrived by now\n", row[0], row[2]);
			else
				fprintf(stdout, "order '%03s' late on arrival! ETA was %.10s but arrived at %.10s\n", row[0], row[2], row[3]);
			//fprintf(stdout, "%s %s %s %s\n", row[0], row[1], row[2], row[3]);
		}

		fprintf(stdout, "\n\n------------these pacakages were not delivered in time------------\n\n\n");

		//  쿼리 실행

	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}

void command_type_7_function() {


	sprintf(command, type_7_query[0]); // customer id 쿼리 찾기
	fprintf(stdout, "------------TYPE 7------------\n\n\n");
	fprintf(stdout, "------------CONTRACTED CUSTOMER BILL------------\n\n\n");

	if (mysql_query(connection, command) != 0) {
		fprintf(stdout, "command: %s\n", command);
		fprintf(stdout, "invalid command!\n");
		printf("%d error : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return;
	}

	MYSQL_RES* result_first = mysql_use_result(connection);
	// 다 찾아야한다 ㅋㅋㅋ 반복문으로 

	if (result_first) { // if there are error in query

		MYSQL_ROW row;
		//fprintf(stdout, "result number %ld!\n", mysql_num_rows(result)); 이거는 store result랑 함께 쓰여야함

		while ((row = mysql_fetch_row(result_first))) {
			//fprintf(stdout, "%s\n", row[0]);
			fprintf(stdout, "%s %s %s\n", row[0], row[1], row[2]);
			//fprintf(stdout, "%s %s %s %s\n", row[0], row[1], row[2], row[3]);
		}

		fprintf(stdout, "\n\n------------CONTRACTED CUSTOMER BILL------------\n\n\n");

		//  쿼리 실행

	}
	else {
		fprintf(stderr, "Error: %s\n", mysql_error(connection));
	}

	return;
}


void run_query(int command_number) {
	if (!command_number)
		return;
	assert(command_number != 0);

	mysql_query(connection, use_schema);

	switch (command_number) {
	case 1:
		command_type_1_function();
		break;
	case 2:
		command_type_2_function();
		break;
	case 3:
		command_type_3_function();
		break;
	case 4:
		command_type_4_function();
		break;
	case 5:
		command_type_5_function();
		break;
	case 6:
		command_type_6_function();
		break;
	case 7:
		command_type_7_function();
		break;
	default:
		fprintf(stdout, "invalid command!\n");

	}




	return;
}

int if_subquery(int target) {
	for (int i = 0; i < sizeof(subquery_command_list) / sizeof(int); i++)
		if (target == subquery_command_list[i])
			return subquery_command_list[i];

	return 0;
}


void command_prompt() {

	int user_command_number = 0;

	while (1) {
		fprintf(stdout, prompt[0]);
		fscanf(stdin, "%d", &user_command_number);

		if (user_command_number == 0)
			break;

		assert(user_command_number != 0);

		run_query(user_command_number);

		//run_subquery(if_subquery(user_command_number));

	}

}
