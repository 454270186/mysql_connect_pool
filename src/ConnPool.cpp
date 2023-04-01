#include "ConnPool.hpp"
#include <iostream>
using namespace Json;
using namespace std;

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
    thread producer(&ConnPool::produce_conn, this);
    // consumer thread
    thread consumer(&ConnPool::recycle_conn, this);

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
    fstream ifs("./db_conf.json");
    if (ifs.is_open()) {
        cout << "file is open" << endl;
    }
    Reader rd;
    Value root;
    rd.parse(ifs, root);
    
    if (root.isObject()) {
        m_db.ip = root["ip"].asString();
        m_db.user = root["user"].asString();
        m_db.password = root["password"].asString();
        m_db.db_name = root["db_name"].asString();
        m_db.port = root["port"].asInt();
        m_max_size = root["max_size"].asInt();
        m_min_size = root["min_size"].asInt();
        m_timeout = root["timeout"].asInt();
        m_max_idle_time = root["max_idle_time"].asInt();


        return true;
    }

    return false;
}

void ConnPool::add_conn() {
    MysqlConn* conn = new MysqlConn();
    if (!conn->connect(m_db.ip, m_db.user, m_db.password, m_db.db_name, m_db.port)) {
        std::cerr << "连接数据库失败" << std::endl;
        delete conn;
        return;
    }
    conn->refresh_alive_time();
    m_conn_q.push(conn);
}

void ConnPool::produce_conn() {
    while (true) {
        {
            unique_lock<mutex> lock(m_mtx);
            m_pro_cond.wait(lock, [this] { return this->m_conn_q.size() < this->m_max_size; });
            add_conn();
        }

        m_cons_cond.notify_all();
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

void ConnPool::recycle_conn() {
    while (true) {
        {
            unique_lock<mutex> lock(m_mtx);
            //cout << "recycle thread" << this_thread::get_id() << endl;
            m_cons_cond.wait(lock, [this] { return this->m_conn_q.size() > this->m_min_size; });

            MysqlConn* mysql_conn = m_conn_q.front();
            if (mysql_conn->get_alive_time() >= m_max_idle_time) {
                m_conn_q.pop();
                delete mysql_conn;
                //cout << "delete a exipired connection, current size: " << m_conn_q.size() << endl;
                m_pro_cond.notify_all();
            }

        }
        this_thread::sleep_for(chrono::milliseconds(300));
    }
}

shared_ptr<MysqlConn> ConnPool::get_conn() {
    unique_lock<mutex> lock(m_mtx);
    
    // 如果队列为空就阻塞
    m_cons_cond.wait(lock, [this] { return !(this->m_conn_q.empty()); });

    shared_ptr<MysqlConn> conn_ptr(m_conn_q.front(), [this](MysqlConn* ptr) {
        unique_lock<mutex> lock(this->m_mtx);
        ptr->refresh_alive_time();
        this->m_conn_q.push(ptr);
        //cout << "return a conntion, current size: " << this->m_conn_q.size() << endl;
    });

    m_conn_q.pop();
    m_pro_cond.notify_all();
    return conn_ptr;
}