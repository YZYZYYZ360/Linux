## 日志系统升级记录
### 原始C实现
```c

// log_demo.cpp 
#include <fcntl.h>   // open()
#include <unistd.h>  // write()
#include <cstring>   // strlen()

int main() {
    // 1. 创建/打开日志文件（系统调用演示）
    int log_fd = open("system.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    
    // 2. 写入日志内容
    const char* msg = "Linux系统编程实战日志\n";
    write(log_fd, msg, strlen(msg));
    
    // 3. 关闭文件描述符
    close(log_fd);
    return 0;
}

## 更新C++实现
```c++

// linux_io/log_demo.cpp
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include<iostream>

int main() {
    // 1. 打开日志文件（C++ RAII风格）
    std::ofstream log_file("system.log", std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "打开文件失败" << std::endl;
        return 1;
    }

    // 2. 获取当前时间（C++11时间库）
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    // 3. 格式化输出（类型安全）
    log_file << "["
             << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
             << "] C++标准库日志记录\n";

    // 4. 文件自动关闭（依赖RAII）
    return 0;
}