#ifndef RENDER_FUNCTIONS
#define RENDER_FUNCTIONS

void drawBatteryLevel();
void drawMinNumbers();
void drawMinTicks();
void drawSecondNumbers();
void drawSecondTicks();
double getBatteryVoltage();

void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);

#endif