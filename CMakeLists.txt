cmake_minimum_required(VERSION 3.20)


project(RailShooterGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)



#这行代码会自动扫描 src/ 文件夹以及它所有的子文件夹
#把所有找到的 .cpp 文件都添加到一个列表(SOURCES)里
#所以大家在添加文件的时候就不需要再修改cmake了！
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 这个和上面一样，不过是针对头文件
# 在代码里你就可以直接写 #include "Core/Game.h"，
# 也不需要再修改cmake了，当你加头文件时
include_directories(src)

# 收集资源文件（.qrc）
file(GLOB_RECURSE RESOURCES "*.qrc")

# 根据找到的所有源文件，创建最终的可执行程序
add_executable(RailShooterGame ${SOURCES} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia)
