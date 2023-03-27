#include "ConnPool.hpp"
using namespace Json;

ConnPool* ConnPool::get_conn_pool() {
    static ConnPool pool;
    return &pool;
}

ConnPool::ConnPool() {
    if (!parseJson()) {
        return;
    }

    for (int i = 0; i < m_min_size; i++) {
        add_conn();
    }

    // producer thread
    thread producer(&produce_conn, this);
    // consumer thread
    thread consumer(&recycle_conn, this);

    producer.detach();
    consumer.detach();
}

ConnPool::~ConnPool() {
    while (!m_conn_q.empty()) {
        MysqlConn* conn = m_conn_q.front();
        m_conn_q.pop();
        delete conn;
    }
}

bool ConnPool::parseJson() {
    fstream ifs("../db_conf.json");

    Reader rd;
    Value root;
    rd.parse(ifs, root);

    if (root.isObject()) {
        m_db.ip = root["ip"].asString();
        m_db.user = root["user"].asString();
        m_db.password = root["password"].asString();
        m_db.db_name = root["db_name"].asString();
        m_db.port = root["port"].asUInt();
        m_max_size = root["max_size"].asInt();
        m_min_size = root["min_size"].asInt();
        m_timeout = root["timeout"].asInt();
        m_max_idle_time = root["max_idle_time"].asInt();

        return true;
    }

    return false;
}

void ConnPool::add_conn() {
    MysqlConn* conn;
    conn->connect(m_db.ip, m_db.user, m_db.password, m_db.db_name, m_db.port);
    conn->refresh_alive_time();
    m_conn_q.push(conn);
}

void ConnPool::produce_conn() {

}

void ConnPool::recycle_conn() {

}