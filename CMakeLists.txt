cmake_minimum_required(VERSION 3.28)

project(mmm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

set(INCLUDES headers/)
set(SOURCES
    src/main.cpp
    src/wgt/MainWidget.cpp
    src/wgt/ContentsWgt.cpp
    src/wgt/TopFuncsWgt.cpp
    src/wgt/file/FileSystemWgt.cpp
    src/wgt/WorkSpace.cpp
    src/map/Mmap.cpp
    src/mobj/MapObject.cpp
    src/mobj/note/Note.cpp
    src/mobj/note/Hold.cpp
    src/mobj/note/Slide.cpp
    src/mobj/note/MixNote.cpp
    headers/plugin/ICanvasRender.cpp
    src/wgt/Canvas.cpp)

find_package(
  Qt6
  COMPONENTS Core Gui Widgets
  REQUIRED)

# 插件列表
set(PLUGINS primaryrender)

# 判断操作系统
if(WIN32)
  set(LIBRARY_SUFFIX ".dll") # Windows
elseif(APPLE)
  set(LIBRARY_SUFFIX ".dylib") # macOS
else()
  set(LIBRARY_SUFFIX ".so") # Linux or other Unix-like OS
endif()

# 设置插件输出路径
set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugin)
# 插件源文件设置
set(primaryrender_SRC headers/plugin/ICanvasRender.cpp src/plugin/CanvasPrimaryRender/CanvasPrimaryRender.cpp)
# 可执行文件
add_executable(mmm ${SOURCES})

target_include_directories(mmm PUBLIC ${INCLUDES})

target_link_libraries(mmm Qt::Core Qt::Gui Qt::Widgets)

# 循环为每个插件设置构建规则
foreach(PLUGIN ${PLUGINS})
  # 动态创建目标
  add_library(${PLUGIN} SHARED ${${PLUGIN}_SRC})

  # 配置生成的动态库属性
  set_target_properties(
    ${PLUGIN}
    PROPERTIES OUTPUT_NAME ${PLUGIN} # 动态库的名称（不带后缀）
               PREFIX "" # 移除默认前缀
               SUFFIX ${LIBRARY_SUFFIX} # 使用根据操作系统设置的后缀
               RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR} # Windows上用于.dll
               LIBRARY_OUTPUT_DIRECTORY
               ${PLUGIN_OUTPUT_DIR} # macOS和Linux上用于.dylib或.so
               ARCHIVE_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR} # 静态库的输出路径
  )
  target_include_directories(${PLUGIN} PUBLIC src/plugin/${${PLUGIN}} headers/plugin)
  target_link_libraries(${PLUGIN} PRIVATE Qt::Core Qt::Gui Qt::Widgets)
endforeach()
