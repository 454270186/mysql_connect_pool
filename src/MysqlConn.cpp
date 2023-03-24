#include "MysqlConn.hpp"

MysqlConn::MysqlConn() {
    m_conn = mysql_init(NULL);

    mysql_set_character_set(m_conn, "utf8");
}

MysqlConn::~MysqlConn() {
    if (m_conn != nullptr) {
        mysql_close(m_conn);
    }

    free_result();
}

bool MysqlConn::connect(string ip, string user, string password, string db_name, unsigned int port) {
    MYSQL* ret = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), password.c_str(), db_name.c_str(), port, NULL, 0);
    
    return ret != nullptr;
}

bool MysqlConn::query(string sql) {
    free_result();
    if (mysql_query(m_conn, sql.c_str())) {
        return false;
    }

    // get the result
    m_res = mysql_store_result(m_conn);
    return true;
}

bool MysqlConn::update(string sql) {
    if (mysql_query(m_conn, sql.c_str())) {
        return false;
    }

    return true;
}

bool MysqlConn::next() {
    if (m_res != nullptr) {
        m_row = mysql_fetch_row(m_res);
        if (m_row != nullptr) {
            return true;
        }
    }

    return false;
}

string MysqlConn::value(int index) {
    int row_cnt = mysql_num_fields(m_res);
    if (index >= row_cnt || index < 0) {
        return string();
    }

    // mysql_fetch_lengths()仅对结果集的当前行有效。
    // 如果在调用mysql_fetch_row()之前或检索了结果集中的所有行后调用了它，将返回NULL。
    char* ans = m_row[index];
    unsigned long length = mysql_fetch_lengths(m_res)[index];

    return string(ans, length);
}

bool MysqlConn::transaction() {
    // 禁用autocommit
    return mysql_autocommit(m_conn, false);
}

bool MysqlConn::commit() {
    return mysql_commit(m_conn);
}

bool MysqlConn::rollback() {
    return mysql_rollback(m_conn);
}

void MysqlConn::refresh_alive_time() {
    m_alive_time = std::chrono::steady_clock::now();
}

// 返回空闲时长
long long MysqlConn::get_alive_time() {
    using namespace std::chrono;
    nanoseconds idle = steady_clock::now() - m_alive_time;
    microseconds res = duration_cast<microseconds>(idle);

    return res.count();
}

string MysqlConn::get_error() {
    const char* ret = mysql_error(m_conn);

    return string(ret);
}

void MysqlConn::free_result() {
    if (m_res) {
        mysql_free_result(m_res);
        m_res = nullptr;
    }
}