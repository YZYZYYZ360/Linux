# 设定 CMake 最低版本要求为 3.10
cmake_minimum_required(VERSION 3.10)

# 定义项目名称为 CGAL_Demo
project(CGAL_Demo)

# 指定 C++ 标准为 C++ 14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 CGAL 库，若未找到则会报错
find_package(CGAL REQUIRED)

# 查找 GMP 库，若未找到则会报错
find_package(GMP REQUIRED)

# 查找 MPFR 库，若未找到则会报错
find_package(MPFR REQUIRED)

# 包含 CGAL 的使用文件，这个文件定义了使用 CGAL 所需的编译和链接设置
include(${CGAL_USE_FILE})

# 添加一个可执行文件，将 main.cpp 编译成名为 cgal_demo 的可执行文件
add_executable(cgal_demo main.cpp LAR_STL.cpp)

# 将可执行文件 cgal_demo 与 CGAL 库进行链接
target_link_libraries(cgal_demo PRIVATE CGAL::CGAL ${GMP_LIBRARIES} ${MPFR_LIBRARIES})

# 如果使用的是 GNU C++ 编译器，添加编译警告选项
if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(cgal_demo PRIVATE -Wall -Wextra)
endif()    