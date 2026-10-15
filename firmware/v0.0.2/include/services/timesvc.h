#pragma once
#include <time.h>

namespace timesvc {
void begin();                              
bool poll(tm& t, bool& haveTime);
unsigned long uptimeSec();               
}
