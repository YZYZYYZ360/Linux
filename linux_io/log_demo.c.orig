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