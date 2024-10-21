#include "../log/Logger.h"
#include "../map/meta/BaseMeta.h"
#include "../mobj/timing/Timing.h"
#include "MapReaderPlugin.h"
#include "note/Hold.h"
#include "note/MixNote.h"
#include "note/Slide.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>

class ByteBuffer {
public:
  enum ByteOrder { LITTLEENDIAN, BIGENDIAN };
  ByteBuffer(const std::vector<char> &data) : data(data), position(0) {}

  void setPosition(size_t pos) { position = pos; }

  void setByteOrder(ByteOrder order) {
    // 可以根据需要实现字节序设置
    this->order = order;
  }
  int getInt() {
    int value = *reinterpret_cast<int *>(&data[position]);
    position += sizeof(int);
    return (order == ByteOrder::LITTLEENDIAN) ? value : swapEndian(value);
  }

  double getDouble() {
    double value = *reinterpret_cast<double *>(&data[position]);
    position += sizeof(double);
    return (order == ByteOrder::LITTLEENDIAN) ? value : swapEndian(value);
  }

  uint8_t get() { return static_cast<uint8_t>(data[position++]); }

  short getShort() {
    short value = *reinterpret_cast<short *>(&data[position]);
    position += sizeof(short);
    return (order == ByteOrder::LITTLEENDIAN) ? value : swapEndian(value);
  }

private:
  std::vector<char> data;
  size_t position;
  ByteOrder order;

  int swapEndian(int value) {
    return ((value >> 24) & 0xff) |      // byte 3
           ((value << 8) & 0xff0000) |   // byte 2
           ((value >> 8) & 0xff00) |     // byte 1
           ((value << 24) & 0xff000000); // byte 0
  }

  double swapEndian(double value) {
    // 实现双精度浮点数的字节序交换
    // 这里假设你实现了对double的字节序处理
    uint64_t intValue;
    std::memcpy(&intValue, &value, sizeof(double));
    intValue = ((intValue >> 56) & 0xff) |
               ((intValue << 40) & 0xff000000000000) |
               ((intValue >> 24) & 0xff000000) | ((intValue << 8) & 0xff0000) |
               ((intValue >> 8) & 0xff00) | ((intValue << 24) & 0xff00000000) |
               ((intValue << 40) & 0xff0000000000) |
               ((intValue >> 56) & 0xff00000000000000);
    std::memcpy(&value, &intValue, sizeof(double));
    return value;
  }

  short swapEndian(short value) { return (value >> 8) | (value << 8); }
};

class ImdMapReader : public MapReaderPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID MapReaderPlugin_iid)
  Q_INTERFACES(MapReaderPlugin)

public:
  ImdMapReader() : MapReaderPlugin(){};
  ~ImdMapReader() override = default;

  virtual Mmap *read(std::string file) override {
    LOG_INFO("开始读取");
    Mmap *map = new Mmap(*_meta);
    // 读取文件
    std::ifstream inputFile(file, std::ios::binary);
    if (!inputFile)
      LOG_ERROR("[" + file + "]" + "文件无法打开");
    // 读取文件数据到缓冲区
    std::vector<char> data((std::istreambuf_iterator<char>(inputFile)),
                           std::istreambuf_iterator<char>());
    inputFile.close();
    ByteBuffer bf(data);
    bf.setByteOrder(ByteBuffer::ByteOrder::LITTLEENDIAN);
    // 校验文件名
    std::string fileName = file.substr(file.find_last_of("/\\") + 1);
    if (!endsWith(fileName, ".imd") || countOccurrences(fileName, '_') != 2) {
      LOG_ERROR("imd文件错误");
    }
    // 找到下划线的位置
    size_t fst_ = fileName.find("_") + 1;
    size_t scd_ = fileName.rfind("_") + 1;
    // 找到最后一个.的位置
    size_t lstdot = fileName.rfind(".");
    // 提取子字符串
    std::string keysinfo = fileName.substr(fst_, scd_ - fst_ + 1);
    std::string audiofile = fileName.substr(0, fst_ - 1);
    std::string audio_file_path =
        file.substr(0, file.find_last_of("/\\")) + "/" + audiofile + ".mp3";
    std::string version = fileName.substr(scd_, lstdot - scd_);
    // 替换 "k" 为 ""
    keysinfo.erase(std::remove(keysinfo.begin(), keysinfo.end(), 'k'),
                   keysinfo.end());
    // 转换为整数
    int maxOrbit = std::stoi(keysinfo);
    LOG_DEBUG("谱面key数[" + std::to_string(maxOrbit) + "]");

    // 读取imd元数据
    int mapLength = bf.getInt();
    int timingAmount = bf.getInt();
    bf.setPosition(8 + timingAmount * 12 + 2);
    int tabRows = bf.getInt();

    // 读取基准 BPM
    bf.setPosition(12);
    double preferenceBpm = bf.getDouble();
    // LOG_DEBUG("谱面基准bpm:[" + std::to_string(preferenceBpm) + "]");

    // 读取 timing 信息
    bf.setPosition(8);
    double tempBpm = 0;
    for (int i = 0; i < timingAmount; ++i) {
      int time = bf.getInt();
      double bpm = bf.getDouble();
      if (bpm != tempBpm) {
        tempBpm = bpm;
        auto t = std::make_shared<Timing>(time, bpm, true);
        map->put_timing(t);
      }
    }
    // 跳过 2 字节
    bf.getShort();

    // 表格行数
    bf.getInt();
    // 读取按键信息
    std::shared_ptr<MixNote> mixNote = nullptr;
    for (int i = 0; i < tabRows; i++) {
      // 按键参数解析
      uint8_t noteTypePar = bf.get();
      bf.get(); // 跳过 1 字节
      int timeStamp = bf.getInt();
      uint8_t complexPar = (noteTypePar & 0xf0) >> 4;
      uint8_t noteType = noteTypePar & 0x0f;
      uint8_t orbit = bf.get() + 1;
      int notePar = bf.getInt();

      // 根据 noteType 初始化对象
      std::shared_ptr<Note> tempNote = nullptr;
      switch (noteType) {
      case 0:
        tempNote = std::make_shared<Note>(timeStamp, orbit);
        break;
      case 1:
        tempNote = std::make_shared<Slide>(timeStamp, orbit, notePar);
        break;
      case 2:
        tempNote = std::make_shared<Hold>(timeStamp, orbit, notePar);
        break;
      }
      // 处理组合键逻辑
      if (complexPar != 0) {
        switch (complexPar) {
        case 0x06:
          mixNote = std::make_shared<MixNote>(timeStamp, orbit);
          mixNote->put_note(tempNote);
          break;
        case 0x02:
          if (mixNote)
            mixNote->put_note(tempNote);
          else
            LOG_WARNING("读取到非法物件");
          break;
        case 0x0A:
          if (mixNote)
            mixNote->put_note(tempNote);
          else
            LOG_WARNING("读取到非法物件");
          map->put_note(mixNote);
          break;
        }
      } else {
        // 普通按键直接添加
        map->put_note(tempNote);
      }
    }
    LOG_INFO("Version:" + version);

    map->meta().setMetaData("AudioFile", MetaType::string_, audiofile);
    map->meta().setMetaData("Title", MetaType::string_, audiofile);
    map->meta().setMetaData("TitleUnicode", MetaType::string_, audiofile);
    map->meta().setMetaData("Version", MetaType::string_, version);
    map->meta().setMetaData("Tabrows", MetaType::int_, tabRows);
    map->meta().setMetaData("MapLength", MetaType::int_, mapLength);
    map->meta().setMetaData("AbsoluteAudioPath", MetaType::string_,
                            audio_file_path);
    map->set_length(mapLength);

    return map;
  }
  virtual std::string suffix() override { return "imd"; }
  bool endsWith(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
  }

  int countOccurrences(const std::string &str, char ch) {
    return std::count(str.begin(), str.end(), ch);
  }
};

#include "ImdMapReader.moc"
