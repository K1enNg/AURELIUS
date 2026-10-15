#pragma once
#include <time.h>
#include "hw/buttons.h"


namespace screens {

struct Def {
  void (*render)(const tm& t, bool haveTime, bool full);
  void (*onButton)(Btn b);
};

extern const Def menuScreen;
extern const Def clockScreen;
extern const Def weatherScreen;
extern const Def lightsScreen;
extern const Def soundScreen;
extern const Def networkScreen;
extern const Def systemScreen;

}  // namespace screens
