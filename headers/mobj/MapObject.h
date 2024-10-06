#ifndef MMM_MAPOBJECT_H
#define MMM_MAPOBJECT_H

#include <sstream>
#include <string>

class MapObject {
public:
  // 物件时间戳
  inline int time() { return _time; };

  // 相对位置
  inline double pos() { return _pos; };
  virtual std::string toString() const;

protected:
  // 时间戳值
  int _time;
  // 物件相对位置
  double _pos;

  // 物件构造
  MapObject(int _timev, double _posv);
  virtual ~MapObject();
};

#endif // MMM_MAPOBJECT_H
