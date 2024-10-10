#include "../../headers/map/Mmap.h"
#include "../../headers/log/Logger.h"
#include "mobj/note/Note.h"
#include "mobj/timing/Timing.h"
#include <algorithm>
#include <memory>
#include <string>
using namespace std;

Mmap::Mmap() {}

Mmap::~Mmap() {}

void Mmap::put_note(shared_ptr<Note> note) {
  int ntime = note->time();
  auto it = nmap.find(ntime);
  if (it != nmap.end()) {
    it->second.push_back(note);
    LOG_DEBUG("已包含时间戳:[" + to_string(ntime) + "]\t已添加物件");
  } else {
    nmap[ntime].push_back(note);
    LOG_DEBUG("不包含时间戳:[" + to_string(ntime) + "]\t已创建新键");
    // 查询列表中的第一个大于此物件时间的最大下标并插入时间戳
    ntimestamplist.insert(
        std::upper_bound(ntimestamplist.begin(), ntimestamplist.end(), ntime),
        ntime);
  }
  // 更新轨道数
  if (note->orbit() + 1 >= max_orbit)
    max_orbit = note->orbit();
}

void Mmap::erase_note(shared_ptr<Note> note) {
  auto it = nmap.find(note->time());
  if (it != nmap.end()) {
    // 从时间戳列表移除
    ntimestamplist.erase(
        std::find(ntimestamplist.begin(), ntimestamplist.end(), note->time()));
    LOG_SUCCESS("移除物件:" + note->toString() + "成功");
    if (it->second.size() >= 1) {
      it->second.erase(std::find(it->second.begin(), it->second.end(), note));
      LOG_INFO("时间戳[" + std::to_string(note->time()) + "]还有[" +
               std::to_string((int)it->second.size()) + "]个物件");
    } else {
      nmap.erase(note->time());
      LOG_INFO("时间戳[" + std::to_string(note->time()) + "]无其他物件");
    }
  } else {
    LOG_WARNING("移除失败,此图中不包含此物件:{" + note->toString() + "}");
  }
}

void Mmap::put_timing(shared_ptr<Timing> timing) {
  int ttime = timing->time();
  auto it = tmap.find(ttime);
  if (it != tmap.end()) {
    it->second.push_back(timing);
  } else {
    tmap[ttime].push_back(timing);
    // 查询列表中的第一个大于此timing时间的最大下标并插入时间戳
    ttimestamplist.insert(
        std::upper_bound(ttimestamplist.begin(), ttimestamplist.end(), ttime),
        ttime);
  }
  if (timing->is_base()) {
    abs_timings.insert({timing->time(), timing});
    LOG_SUCCESS("已添加基准timing:" + timing->toString());
  } else {
    LOG_SUCCESS("已添加timing:" + timing->toString());
  }
}

void Mmap::erase_timing(shared_ptr<Timing> timing) {
  auto it = tmap.find(timing->time());
  if (it != tmap.end()) {
    // 从时间戳列表移除
    ntimestamplist.erase(std::find(ntimestamplist.begin(), ntimestamplist.end(),
                                   timing->time()));
    LOG_SUCCESS("移除timing:{" + timing->toString() + "}成功");
    if (it->second.size() >= 1) {
      it->second.erase(std::find(it->second.begin(), it->second.end(), timing));
      LOG_INFO("时间戳[" + std::to_string(timing->time()) + "]还有[" +
               std::to_string((int)it->second.size()) + "]个timing");
    } else {
      tmap.erase(timing->time());
      LOG_INFO("时间戳[" + std::to_string(timing->time()) + "]无其他timing");
    }
  } else {
    LOG_WARNING("移除失败,此图中不包含此timing:{" + timing->toString() + "}");
  }
}

void Mmap::set_length(int l) { length_ = l; }
