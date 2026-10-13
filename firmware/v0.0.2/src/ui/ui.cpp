#include "ui/ui.h"
#include "ui/screens/screens.h"
#include "hw/sound.h"

namespace ui {

// Order must match enum Screen.
static const screens::Def* const TABLE[SCR_COUNT] = {
  &screens::menuScreen,
  &screens::clockScreen,
  &screens::weatherScreen,
  &screens::lightsScreen,
  &screens::soundScreen,
  &screens::networkScreen,
  &screens::systemScreen,
};

static Screen s_current = SCR_MENU;
static bool s_redraw = true;

Screen current()     { return s_current; }
void requestRedraw() { s_redraw = true; }

void goTo(Screen s) {
  s_current = s;
  s_redraw = true;
  sound::beep(30);
}

void handleButton(Btn b) {
  if (b == B_BACK && s_current != SCR_MENU) { goTo(SCR_MENU); return; }
  TABLE[s_current]->onButton(b);
}

void render(const tm& t, bool haveTime) {
  TABLE[s_current]->render(t, haveTime, s_redraw);
  s_redraw = false;
}

}
