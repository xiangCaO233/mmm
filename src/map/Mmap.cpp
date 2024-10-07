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

void Mmap::putNote(shared_ptr<Note> note) {
  int ntime = note->time();
  auto it = nmap.find(ntime);
  if (it != nmap.end()) {
    it->second.push_back(note);
    LOG_DEBUG("已包含时间戳:" + to_string(ntime) + "\t已添加物件");
  } else {
    nmap[ntime].push_back(note);
    LOG_DEBUG("不包含时间戳:" + to_string(ntime) + "\t已创建新键");
    // 查询列表中的第一个大于此物件时间的最大下标并插入时间戳
    ntimestampList.insert(
        std::upper_bound(ntimestampList.begin(), ntimestampList.end(), ntime),
        ntime);
  }
}

void Mmap::eraseNote(shared_ptr<Note> note) {}

void Mmap::insertTiming(shared_ptr<Timing> timing) {}

void Mmap::eraseTiming(shared_ptr<Timing> timing) {}
