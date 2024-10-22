cmake_minimum_required(VERSION 3.28)

project(mmm)

# c++标准17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)
# 导出编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 添加相对目录宏定义(日志)
add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

# 找依赖包
find_package(
  Qt6
  COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets
  REQUIRED)

include_directories(include src)
set(SOURCES src/main.cpp)

# 判断操作系统
if(WIN32)
  # Windows
  set(LIBRARY_SUFFIX ".dll")
elseif(APPLE)
  # macOS
  set(LIBRARY_SUFFIX ".dylib")
else()
  # Linux
  set(LIBRARY_SUFFIX ".so")
endif()

set(BASS_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/libbass${LIBRARY_SUFFIX}")

add_executable(mmm ${SOURCES})
target_link_libraries(mmm PUBLIC Qt::Core Qt::Widgets Qt::OpenGL
                                 Qt::OpenGLWidgets)
