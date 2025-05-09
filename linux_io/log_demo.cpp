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