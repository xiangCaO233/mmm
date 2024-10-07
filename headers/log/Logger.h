#ifndef MMM_LOGGER_H
#define MMM_LOGGER_H

#ifndef PROJECT_DIR
#define PROJECT_DIR ""
#endif // PROJECT_DIR

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

namespace Logger {
// 日志等级
enum Level { INFO, SUCCESS, DEBUG, WARNING, ERROR, FATAL };
// 终端日志颜色映射
static unordered_map<Level, string> shellColorMap{
    {Level::INFO, "\033[37m"},    {Level::SUCCESS, "\033[1;32m"},
    {Level::DEBUG, "\033[1;33m"}, {Level::WARNING, "\033[1;35m"},
    {Level::ERROR, "\033[31m"},   {Level::FATAL, "\033[35m"}};
// 默认等级
static Level currentLevel{Level::INFO};
// 时间格式
static string timePattern{"%Y-%m-%d %H:%M:%S"};
// 启用显示毫秒信息
static bool enableMilliseconds{true};
static string timeColor{"\033[36m"};
static string resetColor{"\033[0m"};
// 裁剪绝对路径，转换为相对于项目目录的路径
static std::string makeRelativeToProject(const std::string &file) {
  std::string projectDir(PROJECT_DIR);
  // 如果项目路径存在于文件路径中，则去掉它，返回相对路径
  if (file.find(projectDir) == 0) {
    return file.substr(projectDir.length() + 1); // 去掉项目路径的部分
  }
  return file; // 如果没有匹配，则返回原始路径
}
// 打印日志
static void log(Level logLevel, const string &file, int line,
                const string &function, const string &log) {
  // 将文件路径转换为相对项目路径
  string relativeFile = makeRelativeToProject(file);
  // 获取当前时间点
  auto now = chrono::system_clock::now();

  // 获取当前时间的时间戳（毫秒）
  auto milliseconds =
      chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch())
          .count();

  // 转换为可读格式
  time_t now_time_t = chrono::system_clock::to_time_t(now);
  tm now_tm = *localtime(&now_time_t);

  ostringstream oss;
  // 时间信息
  oss << timeColor << '[' << put_time(&now_tm, timePattern.c_str());
  if (enableMilliseconds)
    oss << '.' << setw(2) << setfill('0') << milliseconds % 1000;
  oss << "] ";
  oss << shellColorMap[logLevel] << "[";
  // 日志等级信息
  switch (logLevel) {
  case INFO: {
    oss << "INFO] ";
    break;
  }
  case SUCCESS: {
    oss << "INFO]";
    break;
  }
  case DEBUG: {
    oss << "DEBUG] ";
    break;
  }
  case WARNING: {
    oss << "WARNING] ";
    break;
  }
  case ERROR: {
    oss << "ERROR] ";
    break;
  }
  case FATAL: {
    oss << "FATAL] ";
    break;
  }
  }

  // 其他信息
  oss << "[" << relativeFile << ":" << line << " ::" << function << "())] "
      << log << resetColor << endl;

  cout << oss.str();
};
static void enableMillis() { enableMilliseconds = true; }
static void unableMillis() { enableMilliseconds = false; }
}; // namespace Logger
// 修改日志输出函数，传递调用时的 `__FILE__` 等信息
#define LOG_INFO(li)                                                           \
  log(Logger::Level::INFO, __FILE__, __LINE__, __FUNCTION__, li)
#define LOG_SUCCESS(ls)                                                        \
  log(Logger::Level::SUCCESS, __FILE__, __LINE__, __FUNCTION__, ls)
#define LOG_DEBUG(ld)                                                          \
  log(Logger::Level::DEBUG, __FILE__, __LINE__, __FUNCTION__, ld)
#define LOG_WARNING(lw)                                                        \
  log(Logger::Level::WARNING, __FILE__, __LINE__, __FUNCTION__, lw)
#define LOG_ERROR(le)                                                          \
  log(Logger::Level::ERROR, __FILE__, __LINE__, __FUNCTION__, le)
#define LOG_FATAL(lf)                                                          \
  log(Logger::Level::FATAL, __FILE__, __LINE__, __FUNCTION__, lf)
#endif // MMM_LOGGER_H
