#include <mysql/mysql.h>
#include <iostream>
using namespace std;

int main() {
    MYSQL* mysql = mysql_init(NULL);
    if(mysql == NULL)
    {
        printf("mysql_init() error\n");
        return -1;
    }
    
    printf("success\n");
    return 0;
}