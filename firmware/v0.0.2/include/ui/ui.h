#pragma once
#include <time.h>
#include "hw/buttons.h"

namespace ui {

enum Screen { SCR_MENU, SCR_CLOCK, SCR_WEATHER, SCR_LIGHTS, SCR_SOUND,
              SCR_NETWORK, SCR_SYSTEM, SCR_COUNT };

Screen current();
void goTo(Screen s);                  // switch screen (beeps, forces a full redraw)
void requestRedraw();                 // current screen does a full redraw next frame
void handleButton(Btn b);
void render(const tm& t, bool haveTime);

}
