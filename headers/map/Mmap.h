#ifndef MMM_MMAP_H
#define MMM_MMAP_H

#include "../mobj/note/Note.h"
#include "../mobj/timing/Timing.h"
#include "meta/BaseMeta.h"
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
  std::vector<int> ntimestampList;
  // timing映射
  std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> tmap;
  // timing时间戳列表
  std::vector<int> ttimestamplist;

public:
  Mmap();
  Mmap(std::string &&file);
  ~Mmap();

  // 获取物件映射
  inline std::unordered_map<int, std::vector<std::shared_ptr<Note>>> &
  note_map() {
    return nmap;
  }

  // 获取物件时间戳列表
  inline std::vector<int> &note_timestamps() { return ntimestampList; }

  // 获取timing映射
  inline std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> &
  timing_map() {
    return tmap;
  }

  // 获取timing时间戳列表
  inline std::vector<int> &timing_timestamps() { return ttimestamplist; }
  // 设置物件,包括添加
  void putNote(std::shared_ptr<Note> note);
  // 擦除物件
  void eraseNote(std::shared_ptr<Note> note);

  // 插入timing
  void insertTiming(std::shared_ptr<Timing> timing);
  // 擦除timing
  void eraseTiming(std::shared_ptr<Timing> timing);
};

#endif // MMM_MMAP_H
