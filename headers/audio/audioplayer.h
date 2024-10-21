#ifndef MMM_AUDIOPLAYER_H
#define MMM_AUDIOPLAYER_H

#include "bass.h"
#include <QObject>
#include <string>
#include <unordered_map>

class audioplayer : public QObject {
  // 音频流句柄表
  std::unordered_map<std::string, HSTREAM> audio_streams;
  // 音频位置信息表
  std::unordered_map<std::string, long> audio_pos;
  // 音频属性表
  std::unordered_map<std::string, std::unordered_map<std::string, float>>
      audio_attributes;
  // 音频播放标识表
  std::unordered_map<std::string, bool> audio_is_playing;
  // 设备信息{选中的设备1-默认}{设备数量}{设备信息}
  int selected_device = 1;
  int device_count{0};
  BASS_DEVICEINFO info;

public:
  explicit audioplayer();
  ~audioplayer();

  // 是否正在播放音频
  bool is_playing();
  bool is_playing(std::string &&audio_name);
  // 是否包含一个音频
  bool contains(std::string &&audio_name);
  // 设置播放器采样率
  void set_player_samplerate(int samplerate);
  // 设置播放设备
  void set_play_device(int device_index);
  // 添加和删除音频
  void put_audio(std::string &&audio_file_name);
  void remove_audio(std::string &&audio_file_name);

public slots:
  // 播放和暂停音频
  void play_audio(std::string &&audio_name, bool restart);
  void pause_audio(std::string &&audio_name);
  // 设置音频播放位置
  void set_audio_pos(std::string &audio_name, long time_ms);
  // 设置音频音量大小
  void set_audio_volume(std::string &&audio_name, float volume);
  // 获取
  long get_audio_pos(std::string &&audio_name);
  float get_audio_volume(std::string &&audio_name);
};

#endif // MMM_AUDIOPLAYER_H
