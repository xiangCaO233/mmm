#include "../../headers/audio/audioplayer.h"
#include "../../headers/log/Logger.h"
#include <cstddef>
#include <iterator>

using namespace std;

audioplayer::audioplayer() {
  bool init_ret{true};
  LOG_INFO("可用的音频设备列表:");
  while (BASS_GetDeviceInfo(device_count, &info)) {
    LOG_INFO(to_string(device_count) + ":" + info.name);
    if (info.flags & BASS_DEVICE_DEFAULT) {
      LOG_INFO(string("已选择默认设备:") + info.name);
    }
    device_count++;
  }

  bool ret = BASS_Init(1, 44100, 0, 0, nullptr);
  if (!ret) {
    LOG_ERROR("Bass初始化失败");
    init_ret = false;
  } else {
    LOG_SUCCESS("Bass初始化成功");
  }

  bool flac_plug_ret{false};
  bool wv_plug_ret{false};
  // 加载插件
#ifdef _WIN32
  flac_plug_ret = BASS_PluginLoad("libbassflac.dll", 0);
  wv_plug_ret = BASS_PluginLoad("libbasswv.dll", 0);
#endif //_WIN32
#ifdef __APPLE__
  flac_plug_ret = BASS_PluginLoad("libbassflac.dylib", 0);
  wv_plug_ret = BASS_PluginLoad("libbasswv.dylib", 0);
#endif //__APPLE__
#ifdef __linux__
  flac_plug_ret = BASS_PluginLoad("libbassflac.so", 0);
  wv_plug_ret = BASS_PluginLoad("libbasswv.so", 0);
#endif //__linux__
  if (!flac_plug_ret) {
    LOG_ERROR("加载flac/ogg插件失败");
    init_ret = false;
  } else {
    LOG_SUCCESS("加载flac/ogg插件成功");
  }
  if (!wv_plug_ret) {
    LOG_ERROR("加载wv插件失败");
    init_ret = false;
  } else {
    LOG_SUCCESS("加载wv插件成功");
  }

  if (!init_ret) {
    LOG_ERROR("(bass)音频引擎启动失败");
  } else {
    LOG_SUCCESS("(bass)音频引擎启动成功");
  }
}

audioplayer::~audioplayer() {}

// 是否正在播放
bool audioplayer::is_playing() {
  if (audio_streams.empty()) {
    LOG_WARNING("播放器为空");
    return false;
  }
  for (auto &pair : audio_is_playing) {
    if (pair.second)
      return true;
  }
  return false;
}

bool audioplayer::is_playing(string &&audio_name) {
  if (audio_is_playing.empty()) {
    LOG_WARNING("播放列表为空");
    return false;
  }
  auto it = audio_is_playing.find(audio_name);
  if (it == audio_is_playing.end()) {
    LOG_WARNING("播放列表不存在[" + audio_name + "]");
    return false;
  } else {
    if (it->second)
      return true;
    else
      return false;
    ;
  }
}

// 设置播放设备
void audioplayer::set_play_device(int device_index) {}

bool audioplayer::contains(string &&audio_name) {
  if (audio_streams.empty()) {
    return false;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    return false;
  }
  return true;
}

// 添加音频到播放器
void audioplayer::put_audio(string &&audio_file_name) {
  HSTREAM stream = BASS_StreamCreateFile(FALSE, audio_file_name.c_str(), 0, 0,
                                         BASS_SAMPLE_FLOAT);
  size_t slash_pos = audio_file_name.find_last_of("/\\") + 1;
  string audio_name = audio_file_name.substr(
      slash_pos, audio_file_name.find_last_of('.') - slash_pos);
  if (!stream) {
    LOG_ERROR("添加音频:[" + audio_file_name + "]失败,请检查音频路径绑定");
  } else {
    LOG_SUCCESS("添加音频:[" + audio_file_name + "]成功");
  }

  LOG_INFO("audio_name:" + audio_name);
  LOG_INFO("audio_file_name:" + audio_file_name);
  audio_streams.insert({audio_name, stream});
}

// 移除音频
void audioplayer::remove_audio(string &&audio_name) {
  if (audio_streams.empty()) {
    LOG_ERROR("播放器为空");
    return;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    LOG_ERROR("播放器不存在[" + audio_name + "]");
    return;
  }
  if (audio_is_playing[audio_name]) {
    BASS_ChannelPause(it->second);
  }
  // 移除音频记录
  audio_is_playing.erase(audio_name);
  audio_pos.erase(audio_name);
  // 释放音频流
  BASS_StreamFree(it->second);
  audio_streams.erase(audio_name);
}

// 播放音频(槽)
void audioplayer::play_audio(string &&audio_name, bool restart) {
  if (audio_streams.empty()) {
    LOG_ERROR("播放器为空");
    return;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    LOG_ERROR("播放器不存在[" + audio_name + "]");
    return;
  }
  if (audio_is_playing[it->first]) {
    LOG_ERROR("[" + audio_name + "]已在播放");
  } else {
    BASS_ChannelPlay(it->second, restart);
    LOG_SUCCESS("开始播放[" + audio_name + "],位置[" +
                to_string(audio_pos[it->first]) + "ms]");
  }
}
// 暂停音频
void audioplayer::pause_audio(string &&audio_name) {
  if (audio_streams.empty()) {
    LOG_ERROR("播放器为空");
    return;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    LOG_ERROR("播放器不存在[" + audio_name + "]");
    return;
  }
  if (!audio_is_playing[it->first]) {
    LOG_ERROR("[" + audio_name + "]已为暂停状态");
  } else {
    BASS_ChannelPause(it->second);
    LOG_SUCCESS("已暂停[" + audio_name + "],当前位置[" +
                to_string(audio_pos[it->first]) + "ms]");
  }
}

// 设置音频播放位置
void audioplayer::set_audio_pos(string &audio_name, long time_ms) {
  if (audio_streams.empty()) {
    LOG_ERROR("播放器为空");
    return;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    LOG_ERROR("播放器不存在[" + audio_name + "]");
    return;
  }
  if (audio_is_playing[it->first]) {
    pause_audio(it->first.c_str());
  }
  BASS_ChannelSetPosition(
      it->second,
      BASS_ChannelSeconds2Bytes(it->second, ((double)time_ms / 1000.0)),
      BASS_POS_BYTE);
  audio_pos[it->first] = time_ms;
}

// 设置音频音量
void audioplayer::set_audio_volume(string &&audio_name, float volume) {
  if (audio_streams.empty()) {
    LOG_ERROR("播放器为空");
    return;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    LOG_ERROR("播放器不存在[" + audio_name + "]");
    return;
  }
  BASS_ChannelSetAttribute(it->second, BASS_ATTRIB_VOL, volume);
}

long audioplayer::get_audio_pos(string &&audio_name) {
  if (audio_streams.empty()) {
    LOG_ERROR("播放器为空");
    return -1;
  }
  auto it = audio_streams.find(audio_name);
  if (it == audio_streams.end()) {
    LOG_ERROR("播放器不存在[" + audio_name + "]");
    return -1;
  }
  return BASS_ChannelBytes2Seconds(
             it->second, BASS_ChannelGetPosition(it->second, BASS_POS_BYTE)) *
         1000;
}

float audioplayer::get_audio_volume(string &&audio_name) { return 1.0; }
