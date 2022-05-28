#ifndef query_run_H
#define query_run_H

#define DATA_SIZE 40
#define PRODUCT_SIZE 40
#define CUSTOMER_SIZE 40
#define WAREHOUSE_SIZE 40
#define STORE_SIZE 40


int customer_list[15] = { 1, 3, 6, 7, 8, 10, 12, 14, 15, 17, 19, 20, 21, 22, 23 };

bool in_list(int data) {
    for (int i = 0; i < sizeof(customer_list); i++)
        if (data == customer_list[i])
            return true;

    return false;
}

#endif