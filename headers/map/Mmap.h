#ifndef MMM_MMAP_H
#define MMM_MMAP_H

#include "../mobj/note/Note.h"
#include "../mobj/timing/Timing.h"
#include <memory>
#include <unordered_map>
#include <vector>

class Mmap {
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
  ~Mmap();

  inline std::unordered_map<int, std::vector<std::shared_ptr<Note>>> &
  note_map() {
    return nmap;
  }

  inline std::vector<int> &note_timestamps() { return ntimestampList; }

  inline std::unordered_map<int, std::vector<std::shared_ptr<Timing>>> &
  timing_map() {
    return tmap;
  }

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
