cmake_minimum_required(VERSION 3.20)


project(RailShooterGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




#这行代码会自动扫描 src/ 文件夹以及它所有的子文件夹
#把所有找到的 .cpp 文件都添加到一个列表(SOURCES)里
#所以大家在添加文件的时候就不需要再修改cmake了！
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 这个和上面一样，不过是针对头文件
# 在代码里你就可以直接写 #include "Core/Game.h"，
# 也不需要再修改cmake了，当你加头文件时
include_directories(src)


# 根据找到的所有源文件，创建最终的可执行程序
add_executable(RailShooterGame ${SOURCES})