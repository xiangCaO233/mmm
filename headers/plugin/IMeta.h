#ifndef MMM_IMETA_H
#define MMM_IMETA_H

#include "../map/meta/BaseMeta.h"

class IMeta {
public:
  virtual ~IMeta();
  virtual BaseMeta *createMeta();
};

#endif // MMM_IMETA_H
