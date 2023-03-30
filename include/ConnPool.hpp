#ifndef _CONN_POOL
#define _CONN_POOL
#include <fstream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <jsoncpp/json/json.h>
#include "MysqlConn.hpp"

using namespace std;

struct db_info {
    string ip;
    string user;
    string password;
    string db_name;
    unsigned int port;
};

class ConnPool {
public:
    // 对外接口
    static ConnPool* get_conn_pool();
    shared_ptr<MysqlConn> get_conn();

    ConnPool(const ConnPool& other) = delete;
    ConnPool& operator=(const ConnPool& other) = delete;
    ~ConnPool();

private:
    ConnPool();
    bool parseJson();
    void add_conn();

    void produce_conn();
    void recycle_conn();

private:
    // info for database connection
    db_info m_db;

    // limitation of threads in pool
    int m_max_size;
    int m_min_size;

    // time out for idle thread
    int m_timeout;
    int m_max_idle_time;

    // thread
    mutex m_mtx;
    condition_variable m_pro_cond;
    condition_variable m_cons_cond;
    queue<MysqlConn*> m_conn_q;
};

#endif