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

void MysqlConn::free_result() {
    if (m_res) {
        mysql_free_result(m_res);
        m_res = nullptr;
    }
}