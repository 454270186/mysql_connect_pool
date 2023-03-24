#include "MysqlConn.hpp"
#include <iostream>

int main() {
    MysqlConn conn;

    // 连接数据库
    if (!conn.connect("localhost", "xiaofei", "454270186", "mysqlconn", 3306)) {
        std::cerr << "连接数据库失败" << std::endl;
        return -1;
    }

    // 执行查询
    if (conn.query("SELECT * FROM student")) {
        while (conn.next()) {
            std::cout << "id: " << conn.value(0) << ", name: " << conn.value(1) << std::endl;
        }
    } else {
        std::cerr << "查询失败" << std::endl;
    }

    // 执行更新
    if (conn.update("INSERT INTO student (name) VALUES ('xiaofei')")) {
        std::cout << "更新成功" << std::endl;
    } else {
        std::cerr << "更新失败: " << conn.get_error() << std::endl;
    }

    if (conn.query("SELECT * FROM student")) {
        while (conn.next()) {
            std::cout << "id: " << conn.value(0) << ", name: " << conn.value(1) << std::endl;
        }
    } else {
        std::cerr << "查询失败" << std::endl;
    }

    return 0;
}