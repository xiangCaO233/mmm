cmake_minimum_required(VERSION 3.28)

project(mmm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 添加相对目录宏定义(日志)
add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

# 可执行文件头文件目录
set(INCLUDES headers/audio headers)
# 可执行文件源文件
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
    src/mobj/timing/Timing.cpp
    headers/plugin/RenderPlugin.cpp
    headers/plugin/MetaPlugin.cpp
    headers/plugin/MapReaderPlugin.cpp
    src/wgt/Canvas.cpp
    src/audio/audioplayer.cpp
    headers/audio/posreceiver.cpp
    src/opengl/OpenGLCanvas.cpp)

find_package(
  Qt6
  COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets
  REQUIRED)
find_library(LIB_BASS NAMES bass_lib PARTS)
# 插件类型
set(PLUGINTYPES RenderPlugin MetaPlugin MapReaderPlugin)
# 插件定义
set(RenderPlugin_PLUGINS CanvasPrimaryRender)
set(MetaPlugin_PLUGINS ImdMetaData)
set(MapReaderPlugin_PLUGINS ImdMapReader)
# 设置输出目录的根路径
set(OUTPUT_ROOT_DIR "${CMAKE_BINARY_DIR}/plugin")

# 判断操作系统
if(WIN32)
  set(LIBRARY_SUFFIX ".dll") # Windows
elseif(APPLE)
  set(LIBRARY_SUFFIX ".dylib") # macOS
else()
  set(LIBRARY_SUFFIX ".so") # Linux or other Unix-like OS
endif()

set(BASS_LIB_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/bass_lib/libbass${LIBRARY_SUFFIX}")

# 循环遍历每个类别
foreach(PLUGINTYPE IN LISTS PLUGINTYPES)
  message(STATUS "当前插件类型:" ${PLUGINTYPE})
  # 获取当前类别的输出目录
  set(PLUGINTYPE_OUTPUT_DIR "${OUTPUT_ROOT_DIR}/${PLUGINTYPE}")
  # 检查json目录是否存在
  if(NOT EXISTS ${PLUGINTYPE_OUTPUT_DIR}/json)
    # 创建json目录
    file(MAKE_DIRECTORY ${PLUGINTYPE_OUTPUT_DIR}/json)
  endif()
  # 获取该类别的库列表
  set(PLUGINS "${PLUGINTYPE}_PLUGINS")
  foreach(PLUGIN IN LISTS ${PLUGINS})
    message(STATUS "正在预处理插件:" ${PLUGIN})
    # 为每个库创建共享库目标 定义源文件列表
    set(PLUGIN_SOURCES src/plugin/${PLUGIN}.cpp
                       headers/plugin/${PLUGINTYPE}.cpp)

    # 判断插件类型，如果是 MapReaderPlugin，添加谱面相关源文件
    if("${PLUGINTYPE}" STREQUAL "MapReaderPlugin")
      list(
        APPEND
        PLUGIN_SOURCES
        src/map/Mmap.cpp
        src/mobj/timing/Timing.cpp
        src/mobj/MapObject.cpp
        src/mobj/note/Note.cpp
        src/mobj/note/Hold.cpp
        src/mobj/note/Slide.cpp
        src/mobj/note/MixNote.cpp
        src/mobj/MapObject.cpp)
    endif()

    # 为每个库创建共享库目标
    add_library(${PLUGIN} SHARED ${PLUGIN_SOURCES})
    target_include_directories(${PLUGIN} PUBLIC headers/plugin)

    # 判断插件类型，如果是 MapReaderPlugin，添加包含MapObject
    if("${PLUGINTYPE}" STREQUAL "MapReaderPlugin")
      target_include_directories(${PLUGIN} PUBLIC headers/mobj)
    endif()

    target_link_libraries(${PLUGIN} Qt::Core)
    if("${PLUGINTYPE}" STREQUAL "RenderPlugin")
      target_link_libraries(${PLUGIN} Qt::Gui Qt::Widgets)
    endif()
    # 移除库的前缀,设置后缀
    set_target_properties(${PLUGIN} PROPERTIES PREFIX "")
    set_target_properties(${PLUGIN} PROPERTIES SUFFIX ${LIBRARY_SUFFIX})
    # 设置输出目录为对应类别文件夹
    set_target_properties(${PLUGIN} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                               ${PLUGINTYPE_OUTPUT_DIR})
  endforeach()
endforeach()

# 可执行文件
add_executable(mmm ${SOURCES})

target_include_directories(mmm PUBLIC ${INCLUDES})

target_link_libraries(mmm PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGL
                                 Qt::OpenGLWidgets ${BASS_LIB_PATH})
