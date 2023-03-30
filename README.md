## Mysql connection pool
Based on:
- C++14
- Mysql c++ connector API
- Jsoncpp
- Singleton Pattern

### Version 1.0 Test
**不使用连接池进行5000次查询**

![test1](./img/test_n_1.png)

**使用连接池进行5000次查询**

![test1](./img/test_y_1.png)

目前还存在连接池析构以及内存泄漏的问题