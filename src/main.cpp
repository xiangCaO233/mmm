#include "../headers/log/Logger.h"
#include "../headers/map/Mmap.h"
#include "../headers/mobj/note/Hold.h"
#include "../headers/mobj/note/MixNote.h"
#include "../headers/wgt/MainWidget.h"
#include "mobj/note/Note.h"
#include "mobj/note/Slide.h"
#include <QApplication>
#include <memory>

using namespace Logger;
int main(int argc, char *argv[]) {
  LOG_INFO("Qt App 加载中...");
  QApplication app(argc, argv);
  LOG_INFO("Qt App加载完成");

  auto w = new MainWidget;
  w->show();

  auto map = Mmap();
  map.putNote(std::make_shared<Note>(100, 2));
  map.putNote(std::make_shared<Hold>(50, 2, 1200));
  map.putNote(std::make_shared<Slide>(100, 0, 1));

  auto mix1 = std::make_shared<MixNote>(50, 3);

  auto hold11 = std::make_shared<Hold>(50, 3, 2000);
  auto slide12 = std::make_shared<Slide>(2050, 3, -1);
  mix1->putnote(hold11);
  mix1->putnote(slide12);
  map.putNote(mix1);

  w->test_map(&map);

  return app.exec();
}
