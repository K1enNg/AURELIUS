#pragma once
#include <time.h>
#include "hw/buttons.h"

// Every screen is a render function + a button handler.
// render(): `full` is true when the whole screen must be repainted
//           (screen just opened, or something requested a redraw).
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
