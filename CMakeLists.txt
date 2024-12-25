# 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和语言
project(Library LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件，并指定源文件

include_directories(lib include)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")


add_executable(Library main.cpp src/Book.cpp src/User.cpp src/File.cpp src/Library.cpp lib/md5.cpp)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)  # 忽略某些安全警告
endif()
