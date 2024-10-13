#ifndef MMM_MMAP_H
#define MMM_MMAP_H

#include "../log/Logger.h"
#include "../mobj/note/Note.h"
#include "../mobj/timing/Timing.h"
#include "meta/BaseMeta.h"
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mmap {
  // 图元数据
  BaseMeta mapMetaData;
  // 物件映射
  std::unordered_map<int, std::vector<std::shared_ptr<Note>>> nmap;
  // 物件时间戳列表
  std::vector<int> ntimestamplist;
  // timing映射
  std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> tmap;
  // timing时间戳列表
  std::vector<int> ttimestamplist;
  // 绝对bpm区间头:bpm
  std::map<int, std::shared_ptr<Timing>> abs_timings_;
  // 轨道数
  int max_orbit{0};
  // 歌曲长度
  int length_{60000};

public:
  Mmap();
  Mmap(std::string &&file);
  ~Mmap();

  inline BaseMeta &meta() { return mapMetaData; }

  // 获取物件映射
  inline std::unordered_map<int, std::vector<std::shared_ptr<Note>>> &
  note_map() {
    return nmap;
  }

  // 获取物件时间戳列表
  inline std::vector<int> &note_timestamps() { return ntimestamplist; }

  // 获取timing映射
  inline std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> &
  timing_map() {
    return tmap;
  }

  inline std::map<int, std::shared_ptr<Timing>> &abs_timings() {
    return abs_timings_;
  }

  // 获取timing时间戳列表
  inline std::vector<int> &timing_timestamps() { return ttimestamplist; }
  inline std::shared_ptr<Timing> base_timing(int time) {
    if (time < 0 || time > length_ + 1000) {
      LOG_ERROR("请求时间非法:[" + std::to_string(time) + "]ms");
      return nullptr;
    }
    // 查找小于等于time的key所处迭代器
    auto it = abs_timings_.upper_bound(time);
    if (it != abs_timings_.begin())
      --it;
    return it->second;
  }
  inline int maxOrbit() { return max_orbit; }
  inline int length() { return length_; }
  // 设置长度
  void set_length(int l);
  // 设置物件,包括添加
  void put_note(std::shared_ptr<Note> note);
  // 擦除物件
  void erase_note(std::shared_ptr<Note> note);
  // 插入timing
  void put_timing(std::shared_ptr<Timing> timing);
  // 擦除timing
  void erase_timing(std::shared_ptr<Timing> timing);
  // 获取最近的一个note所处时间戳,over代表是否可超过此时间
  inline int closest_note_timestamp(int time, bool over) {
    if (ntimestamplist.size() == 0) {
      LOG_WARNING("查找失败,时间戳列表为空");
      return -1;
    }
    if (ntimestamplist.size() == 1) {
      LOG_WARNING("注意:map中仅包含一个物件");
      return ntimestamplist[0];
    }

    auto pos =
        std::upper_bound(ntimestamplist.begin(), ntimestamplist.end(), time);
    if (over)
      return *(pos + 1);
    else
      return *pos;
  };
  // 获取最近的一个timing所处时间戳,over代表是否可超过此时间
  inline int closest_timing_timestamp(int time, bool over) {
    if (ttimestamplist.size() == 0) {
      LOG_WARNING("查找失败,时间戳列表为空");
      return -1;
    }
    if (ttimestamplist.size() == 1) {
      LOG_WARNING("注意:map中仅包含一个物件");
      return ttimestamplist[0];
    }

    auto pos =
        std::upper_bound(ttimestamplist.begin(), ntimestamplist.end(), time);
    if (over)
      return *(pos + 1);
    else
      return *pos;
  };
};

#endif // MMM_MMAP_H
