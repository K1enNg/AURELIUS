#pragma once

namespace lights {

enum Mode { LM_AUTO, LM_OFF, LM_RED, LM_GREEN, LM_BLUE, LM_YELLOW,
            LM_CYAN, LM_MAGENTA, LM_WHITE, LM_PARTY, LM_COUNT };

void begin();                 // pin setup
void startupTest();           // R -> G -> B flash, then restores the current mode
void cycle(int dir);          // next (+1) / previous (-1) mode and apply it
void update();                // call from loop(): drives the party animation
void onWeatherChanged();      // refreshes the LED when in AUTO mode

Mode mode();
const char* modeName();
bool channelOn(int ch);       // 0 = R, 1 = G, 2 = B

}