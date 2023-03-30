#ifndef _MYSQL_CONN
#define _MYSQL_CONN
#include <mysql/mysql.h>
#include <string>
#include <chrono>

using std::string;

// 对mysql API的封装
class MysqlConn {
public:
    MysqlConn(); // create mysql object
    ~MysqlConn(); // release resource

    // for basic CURD
    bool connect(string ip, string user, string password, string db_name, unsigned int port);
    bool query(string sql);
    bool update(string sql);
    bool next();
    string value(int index);
    bool fetch_row();
    bool transaction();
    bool commit();
    bool rollback();

    string get_error();

    // for alive time
    void refresh_alive_time();
    long long get_alive_time();

private:
    void free_result();

    MYSQL* m_conn = nullptr;
    MYSQL_RES* m_res = nullptr;
    MYSQL_ROW m_row;

    std::chrono::steady_clock::time_point m_alive_time;
};

#endif