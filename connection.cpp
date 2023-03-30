#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include "ConnPool.hpp"
#include "MysqlConn.hpp"

using namespace std;

void task() {
    for (int i = 0; i < 5000; i++) {
        // 获取连接池对象
        ConnPool* conn_pool = ConnPool::get_conn_pool();

        // 从连接池中获取连接
        shared_ptr<MysqlConn> conn_ptr = conn_pool->get_conn();
        if (conn_ptr == nullptr) {
            cout << "无法获取连接" << endl;
            return;
        }

        // 执行查询语句
        string query = "SELECT * FROM student;";
        if (!conn_ptr->query(query)) {
            cout << "执行查询失败" << endl;
            return;
        }

        // 获取查询结果
        conn_ptr->fetch_row();
    }
    

    // 等待一段时间
    //this_thread::sleep_for(chrono::seconds(3));
}


int main() {
    clock_t start = clock();
    thread th1(task);
    //thread th2(task);

    th1.join();
    // //th2.join();

    // for (int i = 0; i < 5000; i++) {
    //     MysqlConn conn;
    //     conn.connect("localhost", "xiaofei", "454270186", "mysqlconn", 3306);

    //     string query = "SELECT * FROM student;";
    //     if (!conn.query(query)) {
    //         cout << "执行查询失败" << endl;
    //         return -1;
    //     }

    //     conn.fetch_row();
    // }
    
    clock_t end = clock();

    //cout << end - start << "ms" << endl;
    double time_taken = double(end - start) / CLOCKS_PER_SEC;
    cout << "程序运行时间为：" << fixed << time_taken << setprecision(5);
    cout << " 秒" << endl;

    return 0;
}